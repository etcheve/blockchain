# mini-blockchain — C++

C++17 implementation using OpenSSL for SHA256.

## Build & run

```sh
make
./blockchain
```

## Requirements

- g++ with C++17
- OpenSSL (`brew install openssl`)

## Files

| File | Description |
|---|---|
| `transaction.hpp` | `Transaction` struct: `from`, `to`, `amount` |
| `block.hpp/cpp` | Block data, `compute_hash()`, `set_nonce()` / `set_hash()` setters |
| `miner.hpp/cpp` | `Miner::mine(Block&)` — PoW loop, logs elapsed time |
| `blockchain.hpp/cpp` | Chain management and `is_valid()` |
| `main.cpp` | Example chain with named transactions |
