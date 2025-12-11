subgraph server["🌐 SERVER LAYER"]
    TCP[TCP Server<br/>Port 6379<br/>Accept Connections]
end

subgraph eventloop["⚡ EVENT LOOP LAYER"]
    EL[epoll/kqueue<br/>Event Multiplexer]
    READ[Readable Events<br/>Data Ready]
    WRITE[Writable Events<br/>Buffer Space]
end

subgraph connections["🔌 CONNECTION LAYER"]
    CONN1[Connection 1<br/>Read Buffer<br/>Write Buffer]
    CONN2[Connection 2<br/>Read Buffer<br/>Write Buffer]
    CONNN[Connection N<br/>Read Buffer<br/>Write Buffer]
end

subgraph protocol["📋 PROTOCOL LAYER"]
    PARSER[RESP Parser<br/>*2 $3 GET $5 mykey<br/>→ Command Object]
end

subgraph command["⚙️ COMMAND LAYER"]
    DISPATCH[Command Dispatcher]
    GET[GET Handler]
    SET[SET Handler]
    DEL[DEL Handler]
    INCR[INCR Handler]
    OTHER[Other Commands]
end

subgraph storage["💾 STORAGE LAYER"]
    KV[KVStore Manager<br/>hash key mod N]
end

subgraph shards["🗄️ SHARDED DATA"]
    S0[Shard 0<br/>🔒 mutex<br/>HashMap]
    S1[Shard 1<br/>🔒 mutex<br/>HashMap]
    S2[Shard 2<br/>🔒 mutex<br/>HashMap]
    SN[Shard N<br/>🔒 mutex<br/>HashMap]
end

C1 & C2 & C3 & CN -->|TCP Connection| TCP
TCP -->|Create Handler| EL
EL --> READ
EL --> WRITE
READ --> CONN1 & CONN2 & CONNN
CONN1 & CONN2 & CONNN -->|Feed Bytes| PARSER
PARSER -->|Parsed Command| DISPATCH
DISPATCH --> GET & SET & DEL & INCR & OTHER
GET & SET & DEL & INCR & OTHER -->|Execute| KV
KV -->|Route by hash| S0 & S1 & S2 & SN
S0 & S1 & S2 & SN -.->|Response| KV
KV -.->|Result| DISPATCH
DISPATCH -.->|Format RESP| CONN1 & CONN2 & CONNN
CONN1 & CONN2 & CONNN -.->|Queue Write| WRITE
WRITE -.->|Send| C1 & C2 & C3 & CN

classDef clientStyle fill:#E3F2FD,stroke:#1976D2,stroke-width:3px,color:#000
classDef serverStyle fill:#F3E5F5,stroke:#7B1FA2,stroke-width:3px,color:#000
classDef eventStyle fill:#FFF3E0,stroke:#F57C00,stroke-width:3px,color:#000
classDef connStyle fill:#E8F5E9,stroke:#388E3C,stroke-width:3px,color:#000
classDef protocolStyle fill:#FCE4EC,stroke:#C2185B,stroke-width:3px,color:#000
classDef commandStyle fill:#E0F2F1,stroke:#00796B,stroke-width:3px,color:#000
classDef storageStyle fill:#FFF9C4,stroke:#F9A825,stroke-width:3px,color:#000
classDef shardStyle fill:#FFEBEE,stroke:#D32F2F,stroke-width:3px,color:#000

class C1,C2,C3,CN clientStyle
class TCP serverStyle
class EL,READ,WRITE eventStyle
class CONN1,CONN2,CONNN connStyle
class PARSER protocolStyle
class DISPATCH,GET,SET,DEL,INCR,OTHER commandStyle
class KV storageStyle
class S0,S1,S2,SN shardStyle
