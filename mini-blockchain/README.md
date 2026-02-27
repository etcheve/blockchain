# mini-blockchain

A minimal educational blockchain implemented in C++ and Rust with identical logic.

## Features

- **Transactions** — each block carries a list of `Transaction { from, to, amount }` records
- **SHA-256 hashing** — block hash commits to index, timestamp, all transactions, previous hash, difficulty, and nonce
- **Proof of Work** — configurable difficulty (default: 4 leading zeros); a `Miner` component handles the nonce search
- **Chain validation** — verifies hash integrity, PoW target, and chain linkage for every block

## Components

| Component | Responsibility |
|---|---|
| `Transaction` | Data record: sender, receiver, amount |
| `Block` | Immutable block data + `compute_hash()` |
| `Miner` | Proof-of-Work loop; logs mine time to stdout |
| `Blockchain` | Chain management and validation |

## Hash payload

```
SHA256(index + timestamp + transactions + previous_hash + difficulty + nonce)
```

Transactions are serialized as the concatenation of `from + to + amount` for each entry.

## Structure

```
mini-blockchain/
├── cpp/    — C++17, OpenSSL SHA256
└── rust/   — Rust 2021, sha2 + hex crates
```

See each subfolder's README for build instructions.
