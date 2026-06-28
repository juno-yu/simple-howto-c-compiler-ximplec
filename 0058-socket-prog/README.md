# Lesson 0058: Socket Programming

## Status: 📋 Planned | Phase: Stdlib Tier B | Effort: Medium (12-16h)

## Objective

Implement socket API for network programming.

## Socket Programming Overview

```mermaid
flowchart TD
    A[Socket Programming] --> B[Server Side]
    A --> C[Client Side]

    B --> D[socket]
    B --> E[bind]
    B --> F[listen]
    B --> G[accept]
    B --> H[send/recv]

    C --> I[socket]
    C --> J[connect]
    C --> K[send/recv]

    D -->|"socket(domain, type, proto)"| L[Create socket]
    E -->|"bind(fd, addr, len)"| M[Bind to address]
    F -->|"listen(fd, backlog)"| N[Listen for connections]
    G -->|"accept(fd, addr, len)"| O[Accept connection]
    J -->|"connect(fd, addr, len)"| P[Connect to server]
```

## Client-Server Flow

```mermaid
sequenceDiagram
    participant Client
    participant Server

    Server->>Server: socket()
    Server->>Server: bind()
    Server->>Server: listen()
    Client->>Client: socket()
    Client->>Server: connect()
    Server->>Server: accept()
    Client->>Server: send()
    Server->>Client: recv()
    Server->>Client: send()
    Client->>Server: recv()
```

## Functions

| Function | Complexity |
|----------|------------|
| `socket(type, domain, proto)` | Easy |
| `bind(fd, addr, len)` | Medium |
| `listen(fd, backlog)` | Easy |
| `accept(fd, addr, len)` | Medium |
| `connect(fd, addr, len)` | Medium |
| `send/recv` | Medium |

## Implementation Checklist

- [ ] Implement socket syscall 41
- [ ] Implement bind syscall 49
- [ ] Implement listen syscall 50
- [ ] Implement accept syscall 43
- [ ] Implement connect syscall 42
- [ ] Implement send/recv
- [ ] Define sockaddr_in structure
- [ ] Test: simple TCP server that echoes messages
