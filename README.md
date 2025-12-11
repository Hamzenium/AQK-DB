```mermaid
graph TB
    subgraph CLIENT["CLIENT LAYER"]
        C1[Client 1]:::client
        C2[Client 2]:::client
        C3[Client 3]:::client
        CN[Client N]:::client
    end

    subgraph SERVER["🟦 SERVER LAYER - Boost.Asio Networking"]
        TCP[Server Class<br/>tcp::acceptor on port 6379<br/>async_accept connections<br/>Create Connection objects]:::server
    end

    subgraph EVENTLOOP["🔶 EVENT LOOP LAYER - io_context"]
        EPOLL[io_context.run<br/>Handles epoll/kqueue<br/>Non-blocking I/O multiplexing]:::eventloop
        READ[READABLE Events<br/>async_read_some<br/>Data ready]:::event
        WRITE[WRITABLE Events<br/>async_write<br/>Send response]:::event
        EPOLL --- READ
        EPOLL --- WRITE
    end

    subgraph CONNLAYER["CONNECTION LAYER"]
        CONN1[Connection 1<br/>Read Buffer<br/>Write Buffer]:::connection
        CONN2[Connection 2<br/>Read Buffer<br/>Write Buffer]:::connection
        CONNN[Connection N<br/>Read Buffer<br/>Write Buffer]:::connection
    end

    subgraph PROTOCOL["PROTOCOL LAYER"]
        RESP[RESP Parser<br/>Parse byte streams<br/>Validate syntax<br/>Extract commands]:::protocol
    end

    subgraph COMMAND["COMMAND LAYER"]
        DISPATCH[Command Dispatcher]:::dispatcher
        GET[GET]:::cmd
        SET[SET]:::cmd
        DEL[DEL]:::cmd
        INCR[INCR]:::cmd
        OTHER[...]:::cmd
        DISPATCH --> GET
        DISPATCH --> SET
        DISPATCH --> DEL
        DISPATCH --> INCR
        DISPATCH --> OTHER
    end

    subgraph STORAGE["STORAGE LAYER"]
        MGR[KVStore Manager<br/>hash key mod N]:::storage
        SHARD0[Shard 0<br/>mutex<br/>hash_map]:::shard
        SHARD1[Shard 1<br/>mutex<br/>hash_map]:::shard
        SHARD2[Shard 2<br/>mutex<br/>hash_map]:::shard
        SHARDN[Shard N<br/>mutex<br/>hash_map]:::shard
        MGR --> SHARD0
        MGR --> SHARD1
        MGR --> SHARD2
        MGR --> SHARDN
    end

    C1 & C2 & C3 & CN -.->|TCP| TCP
    TCP --> EPOLL
    READ & WRITE --> CONN1 & CONN2 & CONNN
    CONN1 & CONN2 & CONNN --> RESP
    RESP --> DISPATCH
    GET & SET & DEL & INCR & OTHER --> MGR

    classDef client fill:#4A90E2,stroke:#2E5C8A,stroke-width:2px,color:#fff
    classDef server fill:#E94B3C,stroke:#A63229,stroke-width:2px,color:#fff
    classDef eventloop fill:#F39C12,stroke:#C87F0A,stroke-width:3px,color:#fff
    classDef event fill:#F5B041,stroke:#D68910,stroke-width:2px,color:#333
    classDef connection fill:#9B59B6,stroke:#6C3483,stroke-width:2px,color:#fff
    classDef protocol fill:#1ABC9C,stroke:#138D75,stroke-width:2px,color:#fff
    classDef dispatcher fill:#E74C3C,stroke:#A93226,stroke-width:3px,color:#fff
    classDef cmd fill:#EC7063,stroke:#C0392B,stroke-width:2px,color:#fff
    classDef storage fill:#16A085,stroke:#0E6655,stroke-width:3px,color:#fff
    classDef shard fill:#48C9B0,stroke:#1ABC9C,stroke-width:2px,color:#fff
    ```
