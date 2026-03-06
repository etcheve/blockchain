import { useState, useCallback, Fragment } from 'react'
import ChainBlock from './ChainBlock.jsx'
import '../styles/chain.css'

export default function Chain() {
  const [blocks, setBlocks] = useState([{ id: 0, hash: '' }])

  const handleHashChange = useCallback((index, hash) => {
    setBlocks(prev => prev.map((b, i) => i === index ? { ...b, hash } : b))
  }, [])

  const addBlock = () => {
    setBlocks(prev => [...prev, { id: prev.length, hash: '' }])
  }

  return (
    <div className="chain-scroll">
      <div className="chain">
        {blocks.map((block, i) => (
          <Fragment key={block.id}>
            <ChainBlock
              index={i}
              parentHash={i === 0 ? '0' : blocks[i - 1].hash}
              onHashChange={(h) => handleHashChange(i, h)}
            />
            {i < blocks.length - 1 && (
              <div className="chain-connector">
                <div className="chain-connector-line" />
                <div className="chain-connector-arrow">→</div>
                <span className="chain-connector-hash">
                  {block.hash ? block.hash.slice(0, 8) + '…' : '—'}
                </span>
                <div className="chain-connector-arrow">→</div>
                <div className="chain-connector-line" />
              </div>
            )}
          </Fragment>
        ))}

        <button className="btn-add-block" onClick={addBlock}>
          + Add Block
        </button>
      </div>
    </div>
  )
}
