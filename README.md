# C++ Limit Order Book Simulator

## Overview

This project is a **C++ implementation of a limit order book and matching engine** inspired by the core mechanics used in electronic trading systems.

The simulator supports:

- buy and sell limit orders
- price-time-priority matching
- partial and full fills
- order cancellation
- trade history tracking
- order lifecycle state management
- CSV-based event replay
- a benchmark-ready structure for latency measurement

The goal of this project is to demonstrate strong fundamentals in:

- C++ systems programming
- event-driven design
- matching-engine logic
- trading-system style state management
- extensible architecture for replay and benchmarking

---

## Motivation

Trading firms and market-making firms build systems that:

- receive event streams
- maintain order book state
- match orders correctly
- generate executions
- track order lifecycle changes
- optimize hot paths for low latency

This project is a simplified educational version of those ideas.

Instead of building a generic finance app, this simulator focuses on the **matching engine layer**, which is one of the most important components in electronic markets.

---

## Features

### Core Matching Engine
- Add **buy** orders
- Add **sell** orders
- Match orders using **price-time priority**
- Support **partial fills**
- Support **full fills**
- Track **best bid** and **best ask**

### Order Management
- Cancel existing orders
- Track **original quantity**
- Track **remaining quantity**
- Maintain order lifecycle states:
  - `Open`
  - `PartiallyFilled`
  - `Filled`
  - `Canceled`

### Trade Tracking
- Record every executed trade
- Store:
  - buy order ID
  - sell order ID
  - execution price
  - execution quantity
  - timestamp

### Replay and Testing
- Replay events from a **CSV file**
- Process deterministic event streams
- Validate matching behavior without manual entry

### Extensibility
- Clean structure for adding:
  - modify order support
  - market orders
  - benchmark harness
  - p50 / p95 / p99 latency reporting
  - multi-symbol support
  - unit tests

└── README.md

