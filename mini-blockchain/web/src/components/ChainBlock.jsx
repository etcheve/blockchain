import { useState, useEffect, useRef } from 'react'
import { sha256, serializeTransactions } from '../utils/hash.js'
import TransactionList from './TransactionList.jsx'
import '../styles/block.css'
import '../styles/chain.css'

export default function ChainBlock({ index, parentHash, onHashChange }) {
  const [mode, setMode] = useState('raw')
  const [data, setData] = useState(index === 0 ? 'Genesis Block' : '')
  const [transactions, setTransactions] = useState([])
  const [nonce, setNonce] = useState(0)
  const [hash, setHash] = useState('')
  const [difficulty, setDifficulty] = useState(4)
  const [status, setStatus] = useState('idle')
  const [elapsedMs, setElapsedMs] = useState(null)

  const workerRef = useRef(null)

  const hashInput = mode === 'raw' ? data : serializeTransactions(transactions)

  const stopMining = () => {
    workerRef.current?.terminate()
    workerRef.current = null
    setStatus('idle')
  }

  // Upstream block changed: stop mining + cascade broken state
  useEffect(() => {
    if (workerRef.current) stopMining()
    setStatus(prev => prev === 'valid' ? 'broken' : prev === 'invalid' ? 'idle' : prev)
    setElapsedMs(null)
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [parentHash])

  // This block's content was edited: valid → broken, invalid → idle
  useEffect(() => {
    setStatus(prev => prev === 'valid' ? 'broken' : prev === 'invalid' ? 'idle' : prev)
    setElapsedMs(null)
  }, [hashInput])

  // Recompute hash live when content, nonce, or parentHash changes
  useEffect(() => {
    let cancelled = false
    sha256(hashInput, nonce, parentHash).then(h => {
      if (!cancelled) { setHash(h); onHashChange(h) }
    })
    return () => { cancelled = true }
    // onHashChange excluded: stable via useCallback in Chain
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [hashInput, nonce, parentHash])

  const handleModeChange = (e) => {
    if (workerRef.current) stopMining()
    setMode(e.target.value)
    setNonce(0)
    setStatus('idle')
    setElapsedMs(null)
  }

  const handleDataChange = (e) => {
    if (workerRef.current) stopMining()
    setData(e.target.value)
    setNonce(0)
    // status reset owned by hashInput effect
  }

  const handleTransactionsChange = (newTxs) => {
    if (workerRef.current) stopMining()
    setTransactions(newTxs)
    setNonce(0)
    // status reset owned by hashInput effect
  }

  const handleNonceChange = (e) => {
    if (workerRef.current) return
    setNonce(Math.max(0, Number(e.target.value)))
    setStatus('idle')
    setElapsedMs(null)
  }

  const handleDifficultyChange = (e) => {
    if (workerRef.current) stopMining()
    setDifficulty(Math.max(1, Math.min(6, Number(e.target.value))))
    setStatus('idle')
  }

  const handleValidate = () => {
    if (!hash) return
    setStatus(hash.startsWith('0'.repeat(difficulty)) ? 'valid' : 'invalid')
  }

  const handleMine = () => {
    if (status === 'mining') return
    workerRef.current?.terminate()

    const worker = new Worker(
      new URL('../workers/chain-miner.worker.js', import.meta.url),
      { type: 'module' }
    )
    workerRef.current = worker
    setStatus('mining')
    setElapsedMs(null)

    worker.onmessage = (e) => {
      const msg = e.data
      if (msg.type === 'progress') {
        setNonce(msg.nonce)
        setHash(msg.hash)
        onHashChange(msg.hash)
      } else if (msg.type === 'done') {
        setNonce(msg.nonce)
        setHash(msg.hash)
        setElapsedMs(msg.elapsed_ms)
        setStatus('valid')
        onHashChange(msg.hash)
        worker.terminate()
        workerRef.current = null
      }
    }

    worker.onerror = () => {
      setStatus('idle')
      worker.terminate()
      workerRef.current = null
    }

    worker.postMessage({ data: hashInput, parentHash, difficulty })
  }

  const renderHash = () => {
    if (!hash) return <span className="hash-suffix">—</span>
    const target = '0'.repeat(difficulty)
    if (status === 'valid' && hash.startsWith(target)) {
      return (
        <>
          <span className="hash-prefix-valid">{hash.slice(0, difficulty)}</span>
          <span className="hash-suffix">{hash.slice(difficulty)}</span>
        </>
      )
    }
    if (status === 'mining') {
      let zeros = 0
      while (zeros < hash.length && hash[zeros] === '0') zeros++
      return zeros > 0
        ? (<><span className="hash-prefix-mining">{hash.slice(0, zeros)}</span><span className="hash-suffix">{hash.slice(zeros)}</span></>)
        : <span className="hash-suffix">{hash}</span>
    }
    return <span className="hash-suffix">{hash}</span>
  }

  const statusLabel = { idle: 'Idle', mining: 'Mining…', valid: 'Valid', invalid: 'Invalid', broken: 'Broken' }[status]
  const isGenesis = index === 0

  return (
    <div className={`block-card ${status}`}>
      <div className="block-header">
        <span className="block-title">{isGenesis ? 'Block #0 — Genesis' : `Block #${index}`}</span>
        <span className={`block-status-badge ${status}`}>{statusLabel}</span>
      </div>

      {/* Parent Hash */}
      <div className="field">
        <div className="field-label">Parent Hash</div>
        <div className={`field-parent-hash${isGenesis ? ' genesis' : ''}`}>
          {isGenesis ? '0 (genesis)' : (parentHash || '—')}
        </div>
      </div>

      {/* Data / Transactions */}
      <div className="field">
        <div className="field-label-row">
          <div className="field-label">Data</div>
          <select
            className="mode-select"
            value={mode}
            onChange={handleModeChange}
            disabled={status === 'mining'}
          >
            <option value="raw">Raw</option>
            <option value="transactions">Transactions</option>
          </select>
        </div>
        {mode === 'raw' ? (
          <textarea
            value={data}
            onChange={handleDataChange}
            rows={3}
            spellCheck={false}
          />
        ) : (
          <TransactionList
            transactions={transactions}
            onChange={handleTransactionsChange}
            disabled={status === 'mining'}
          />
        )}
      </div>

      {/* Difficulty */}
      <div className="field">
        <div className="field-label">Difficulty (leading zeros)</div>
        <input
          type="number"
          min={1}
          max={6}
          value={difficulty}
          onChange={handleDifficultyChange}
        />
      </div>

      {/* Nonce */}
      <div className="field">
        <div className="field-label">Nonce</div>
        <input
          type="number"
          min={0}
          value={nonce}
          onChange={handleNonceChange}
          disabled={status === 'mining'}
          style={{ width: '160px' }}
        />
      </div>

      {/* Hash */}
      <div className="field">
        <div className="field-label">Hash (SHA-256)</div>
        <div className="hash-display">{renderHash()}</div>
        {elapsedMs !== null && (
          <div className="elapsed-info">Mined in {elapsedMs.toLocaleString()} ms</div>
        )}
      </div>

      <div className="block-actions">
        <button
          className="btn btn-validate"
          onClick={handleValidate}
          disabled={status === 'mining' || !hash}
        >
          Validate
        </button>
        {status === 'mining' ? (
          <button className="btn btn-stop" onClick={stopMining}>Stop</button>
        ) : (
          <button className="btn btn-mine" onClick={handleMine}>Mine</button>
        )}
      </div>
    </div>
  )
}
