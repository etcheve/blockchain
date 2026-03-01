# Market Connection

Test suite for connecting to crypto exchange APIs and measuring response times. Uses sandbox/testnet environments where available.

## Goal

- Establish connections to major exchanges
- Measure API response times
- Validate authentication flows
- Test market data endpoints (ticker, order book, trades)

## Exchanges

| Exchange  | Sandbox Available | Sandbox URL                          | Docs |
|-----------|:-----------------:|--------------------------------------|------|
| Binance   | ✅                | https://testnet.binance.vision       | [API Docs](https://testnet.binance.vision/) |
| Coinbase  | ✅                | https://api-public.sandbox.exchange.coinbase.com | [API Docs](https://docs.cdp.coinbase.com/exchange/docs/sandbox) |
| Kraken    | ✅                | https://demo-futures.kraken.com      | [API Docs](https://docs.kraken.com/api/) |
| Upbit     | ❌                | — (no official sandbox)             | [API Docs](https://docs.upbit.com/) |

## Structure

```
market-connection/
  binance/     — Binance Spot Testnet
  coinbase/    — Coinbase Exchange Sandbox
  kraken/      — Kraken Demo/Futures Sandbox
  upbit/       — Upbit (production, read-only endpoints)
```

## Authentication

Each exchange requires API keys. Generate sandbox keys from each exchange's testnet portal and store them locally as `*.ignore.*` files (git-ignored).

Example:
```
binance_api.ignore.txt
coinbase_api.ignore.txt
```

## Notes

- Binance testnet resets ~monthly; API keys are preserved across resets
- Upbit has no sandbox; use public market data endpoints only (no auth required for read)
