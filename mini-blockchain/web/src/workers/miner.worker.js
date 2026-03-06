/**
 * Proof-of-Work miner running in a Web Worker.
 *
 * Receives: { data: string, difficulty: number }
 * Posts progress every PROGRESS_INTERVAL iterations:
 *   { type: 'progress', nonce: number, hash: string }
 * Posts final result:
 *   { type: 'done', nonce: number, hash: string, elapsed_ms: number }
 * Posts on stop signal (data === null):
 *   (worker simply ignores new messages — the main thread terminates it)
 */

const PROGRESS_INTERVAL = 5000

async function sha256(data, nonce) {
  const input = data + nonce
  const encoded = new TextEncoder().encode(input)
  const hashBuffer = await crypto.subtle.digest('SHA-256', encoded)
  const hashArray = Array.from(new Uint8Array(hashBuffer))
  return hashArray.map(b => b.toString(16).padStart(2, '0')).join('')
}

self.onmessage = async function (event) {
  const { data, difficulty } = event.data
  const target = '0'.repeat(difficulty)
  const startTime = performance.now()
  let nonce = 0

  while (true) {
    const hash = await sha256(data, nonce)

    if (nonce % PROGRESS_INTERVAL === 0) {
      self.postMessage({ type: 'progress', nonce, hash })
    }

    if (hash.startsWith(target)) {
      const elapsed_ms = Math.round(performance.now() - startTime)
      self.postMessage({ type: 'done', nonce, hash, elapsed_ms })
      return
    }

    nonce++
  }
}
