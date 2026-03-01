# Market Connection

Test suite for connecting to crypto exchange APIs and measuring response times. Uses sandbox/testnet environments where available.

## Goal

- Establish connections to major exchanges
- Measure API response times
- Validate authentication flows
- Test market data endpoints (ticker, order book, trades)

## Exchanges

| Exchange | Environment | Base URL | Sandbox Mode | Docs |
|----------|-------------|----------|:------------:|------|
| Binance  | Testnet | https://testnet.binance.vision | Separate URL | [Docs](https://testnet.binance.vision/) |
| Coinbase | Sandbox | https://api-public.sandbox.exchange.coinbase.com | Separate URL | [Docs](https://docs.cdp.coinbase.com/exchange/docs/sandbox) |
| Kraken   | Production public | https://api.kraken.com/0/public | No sandbox (public endpoints only) | [Docs](https://docs.kraken.com/api/) |
| Upbit    | Production public | https://api.upbit.com/v1 | No sandbox (public endpoints only) | [Docs](https://docs.upbit.com/) |
| OKX      | Simulated trading | https://www.okx.com | Header: `x-simulated-trading: 1` | [Docs](https://www.okx.com/docs-v5/en/) |
| Bybit    | Testnet | https://api-testnet.bybit.com | Separate URL | [Docs](https://bybit-exchange.github.io/docs/v5/intro) |

## Structure

```
market-connection/
  binance/     — Binance Spot Testnet
  coinbase/    — Coinbase Exchange Sandbox
  kraken/      — Kraken (production public endpoints)
  upbit/       — Upbit (production public endpoints)
  okx/         — OKX Simulated Trading
  bybit/       — Bybit Testnet
```

Each exchange folder follows the same layout:
```
<exchange>/
  connection/
    <exchange>_client.hpp   — Response struct + client interface
    <exchange>_client.cpp   — curl HTTP logic
  main.cpp                  — prints server time + BTC ticker
  Makefile
```

## Authentication

Authenticated endpoints (orders, account info) require API keys. Generate sandbox keys from each exchange's testnet portal and store them locally as `*.ignore.*` files (git-ignored).

Example:
```
binance_api.ignore.txt
coinbase_api.ignore.txt
```

## Notes

- Binance testnet resets ~monthly; API keys are preserved across resets
- Kraken spot has no public sandbox; UAT environment requires contacting support
- Upbit has no sandbox; public market data uses KRW (Korean Won) as base currency
- OKX sandbox uses the same production URL — sandbox mode is toggled via request header
