# Architecture Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                         CLIENT LAYER                             │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐        │
│  │ Client 1 │  │ Client 2 │  │ Client 3 │  │ Client N │        │
│  └────┬─────┘  └────┬─────┘  └────┬─────┘  └────┬─────┘        │
└───────┼─────────────┼─────────────┼─────────────┼───────────────┘
        │             │             │             │
        └─────────────┴─────────────┴─────────────┘
                      │ TCP Connections
                      ▼
┌─────────────────────────────────────────────────────────────────┐
│                        SERVER LAYER                              │
│  ┌───────────────────────────────────────────────────────────┐  │
│  │                    TCP Server                              │  │
│  │  • Listen on port (default: 6379)                         │  │
│  │  • Accept incoming connections                            │  │
│  │  • Spawn Connection handlers                              │  │
│  └───────────────────────┬───────────────────────────────────┘  │
└────────────────────────────┼──────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────┐
│                      EVENT LOOP LAYER                            │
│  ┌───────────────────────────────────────────────────────────┐  │
│  │              epoll/kqueue Event Loop                       │  │
│  │  ┌─────────────────┐  ┌─────────────────┐                │  │
│  │  │ READABLE Events │  │ WRITABLE Events │                │  │
│  │  │  • Data ready   │  │  • Buffer space │                │  │
│  │  │  • New commands │  │  • Send response│                │  │
│  │  └────────┬────────┘  └────────┬────────┘                │  │
│  └───────────┼──────────────────────┼───────────────────────────┘
└──────────────┼──────────────────────┼───────────────────────────┘
               │                      │
               ▼                      ▼
┌─────────────────────────────────────────────────────────────────┐
│                    CONNECTION LAYER                              │
│  ┌───────────────┐  ┌───────────────┐  ┌───────────────┐       │
│  │ Connection 1  │  │ Connection 2  │  │ Connection N  │       │
│  │ ┌───────────┐ │  │ ┌───────────┐ │  │ ┌───────────┐ │       │
│  │ │Read Buffer│ │  │ │Read Buffer│ │  │ │Read Buffer│ │       │
│  │ └─────┬─────┘ │  │ └─────┬─────┘ │  │ └─────┬─────┘ │       │
│  │       │       │  │       │       │  │       │       │       │
│  │ ┌─────▼──────┐│  │ ┌─────▼──────┐│  │ ┌─────▼──────┐│       │
│  │ │Write Buffer││  │ │Write Buffer││  │ │Write Buffer││       │
│  │ └────────────┘│  │ └────────────┘│  │ └────────────┘│       │
│  └───────┬───────┘  └───────┬───────┘  └───────┬───────┘       │
└──────────┼──────────────────┼──────────────────┼────────────────┘
           │                  │                  │
           └──────────────────┴──────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────┐
│                      PROTOCOL LAYER                              │
│  ┌───────────────────────────────────────────────────────────┐  │
│  │                    RESP Parser                             │  │
│  │                                                            │  │
│  │  Input:  *2\r\n$3\r\nGET\r\n$5\r\nmykey\r\n             │  │
│  │                      │                                     │  │
│  │                      ▼                                     │  │
│  │  Parse:  ┌─────────────────┐                             │  │
│  │          │ Command: GET    │                             │  │
│  │          │ Args: ["mykey"] │                             │  │
│  │          └────────┬────────┘                             │  │
│  └───────────────────┼───────────────────────────────────────┘  │
└────────────────────────┼──────────────────────────────────────────┘
                        │
                        ▼
┌─────────────────────────────────────────────────────────────────┐
│                     COMMAND LAYER                                │
│  ┌───────────────────────────────────────────────────────────┐  │
│  │               Command Dispatcher                           │  │
│  │                                                            │  │
│  │  ┌──────┐  ┌──────┐  ┌──────┐  ┌──────┐  ┌──────┐       │  │
│  │  │ GET  │  │ SET  │  │ DEL  │  │ INCR │  │ ...  │       │  │
│  │  └──┬───┘  └──┬───┘  └──┬───┘  └──┬───┘  └──┬───┘       │  │
│  └─────┼─────────┼─────────┼─────────┼─────────┼─────────────┘  │
└────────┼─────────┼─────────┼─────────┼─────────┼────────────────┘
         └─────────┴─────────┴─────────┴─────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────┐
│                      STORAGE LAYER                               │
│  ┌───────────────────────────────────────────────────────────┐  │
│  │                    KVStore Manager                         │  │
│  │                                                            │  │
│  │              hash(key) % shard_count                       │  │
│  │                      │                                     │  │
│  │       ┌──────────────┼──────────────┬──────────────┐      │  │
│  └───────┼──────────────┼──────────────┼──────────────┼──────┘  │
└──────────┼──────────────┼──────────────┼──────────────┼──────────┘
           │              │              │              │
           ▼              ▼              ▼              ▼
    ┌──────────┐   ┌──────────┐   ┌──────────┐   ┌──────────┐
    │ Shard 0  │   │ Shard 1  │   │ Shard 2  │   │ Shard N  │
    │┌────────┐│   │┌────────┐│   │┌────────┐│   │┌────────┐│
    ││ mutex  ││   ││ mutex  ││   ││ mutex  ││   ││ mutex  ││
    │└────────┘│   │└────────┘│   │└────────┘│   │└────────┘│
    │┌────────┐│   │┌────────┐│   │┌────────┐│   │┌────────┐│
    ││hash_map││   ││hash_map││   ││hash_map││   ││hash_map││
    │└────────┘│   │└────────┘│   │└────────┘│   │└────────┘│
    └──────────┘   └──────────┘   └──────────┘   └──────────┘
```

## Component Responsibilities

### Client Layer
- External applications connecting via TCP
- Send RESP-formatted commands
- Receive RESP-formatted responses

### Server Layer
- Listen for incoming TCP connections
- Accept and manage client sockets
- Initialize connection handlers

### Event Loop Layer
- Non-blocking I/O multiplexing (epoll/kqueue)
- Monitor socket readability (incoming data)
- Monitor socket writability (buffer space available)
- Efficient handling of thousands of concurrent connections

### Connection Layer
- Per-client state management
- Read buffer: Accumulate incoming bytes
- Write buffer: Queue outgoing responses
- Parse complete RESP messages from stream

### Protocol Layer
- RESP (REdis Serialization Protocol) parser
- Convert byte streams to structured commands
- Validate command syntax
- Handle inline and bulk string formats

### Command Layer
- Route parsed commands to handlers
- Implement Redis commands (GET, SET, DEL, INCR, etc.)
- Execute business logic
- Generate RESP-formatted responses

### Storage Layer
- **KVStore Manager**: Coordinate access across shards
- **Sharding**: Distribute keys using `hash(key) % N`
- **Shards**: Independent hash maps with per-shard locking
- Lock-free reads between shards for parallelism
- Thread-safe operations within each shard

## Data Flow Example

```
Client sends: SET mykey myvalue

1. TCP bytes arrive → Event Loop detects readable socket
2. Connection reads into buffer: "*3\r\n$3\r\nSET\r\n..."
3. RESP Parser extracts: Command("SET", ["mykey", "myvalue"])
4. Dispatcher routes to SET handler
5. SET handler:
   - Computes shard: hash("mykey") % N = shard_id
   - Locks shard_id
   - Inserts/updates map["mykey"] = "myvalue"
   - Unlocks shard_id
6. Response generated: "+OK\r\n"
7. Response written to Connection write buffer
8. Event Loop detects writable socket → sends response
9. Client receives: "+OK\r\n"
```
