/**
 * Serialize a transaction list into the string that gets hashed.
 * Matches the C++ implementation: concatenated from+to+amount strings.
 */
export function serializeTransactions(txs) {
  return txs.map(t => (t.from || '') + (t.to || '') + (t.amount || '')).join('')
}

/**
 * Compute SHA-256 of `data + nonce + parentHash` using the browser-native Web Crypto API.
 * Returns a lowercase 64-character hex string.
 * parentHash defaults to '' so existing single-block callers are unaffected.
 */
export async function sha256(data, nonce, parentHash = '') {
  const input = data + nonce + parentHash
  const encoded = new TextEncoder().encode(input)
  const hashBuffer = await crypto.subtle.digest('SHA-256', encoded)
  const hashArray = Array.from(new Uint8Array(hashBuffer))
  return hashArray.map(b => b.toString(16).padStart(2, '0')).join('')
}
