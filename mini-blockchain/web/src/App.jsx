import { useState } from 'react'
import Block from './components/Block.jsx'
import Chain from './components/Chain.jsx'
import './App.css'

export default function App() {
  const [tab, setTab] = useState('block')

  return (
    <div className="app">
      <header className="app-header">
        <h1 className="app-title">Mini Blockchain</h1>
        <p className="app-subtitle">Proof-of-Work Block Explorer</p>
        <div className="tab-bar">
          <button
            className={`tab-btn ${tab === 'block' ? 'active' : ''}`}
            onClick={() => setTab('block')}
          >
            Single Block
          </button>
          <button
            className={`tab-btn ${tab === 'chain' ? 'active' : ''}`}
            onClick={() => setTab('chain')}
          >
            Blockchain
          </button>
        </div>
      </header>
      <main className="app-main">
        {tab === 'block' ? <Block /> : <Chain />}
      </main>
    </div>
  )
}
