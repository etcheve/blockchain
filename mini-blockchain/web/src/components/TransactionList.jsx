export default function TransactionList({ transactions, onChange, disabled }) {
  const update = (index, field, value) =>
    onChange(transactions.map((tx, i) => i === index ? { ...tx, [field]: value } : tx))

  const remove = (index) =>
    onChange(transactions.filter((_, i) => i !== index))

  const add = () =>
    onChange([...transactions, { from: '', to: '', amount: '' }])

  return (
    <div className="tx-list">
      {transactions.length > 0 && (
        <div className="tx-header">
          <span>From</span>
          <span>To</span>
          <span>Amount</span>
          <span />
        </div>
      )}

      {transactions.map((tx, i) => (
        <div key={i} className="tx-row">
          <input
            value={tx.from}
            onChange={e => update(i, 'from', e.target.value)}
            placeholder="Alice"
            disabled={disabled}
            spellCheck={false}
          />
          <input
            value={tx.to}
            onChange={e => update(i, 'to', e.target.value)}
            placeholder="Bob"
            disabled={disabled}
            spellCheck={false}
          />
          <input
            value={tx.amount}
            onChange={e => update(i, 'amount', e.target.value)}
            placeholder="0.0"
            disabled={disabled}
            className="tx-amount"
            spellCheck={false}
          />
          <button
            className="tx-delete"
            onClick={() => remove(i)}
            disabled={disabled}
            title="Remove transaction"
          >
            ×
          </button>
        </div>
      ))}

      {transactions.length === 0 && (
        <div className="tx-empty">No transactions — add one below</div>
      )}

      <button className="tx-add" onClick={add} disabled={disabled}>
        + Add Transaction
      </button>
    </div>
  )
}
