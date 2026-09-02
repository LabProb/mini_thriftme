# mini_thriftme

A small C++20 demonstration project containing two RPC implementations for the
same vehicle-metrics domain:

- `mini_thriftme_server` and `mini_thriftme_client` implement a lightweight
  TCP RPC exchange.
- `thrift_server` and `thrift_client` use code generated from
  [`proto/vehicle.thrift`](proto/vehicle.thrift) with Apache Thrift.

## Requirements

- CMake 3.16 or newer
- A C++20 compiler
- Apache Thrift compiler (`thrift`)
- Apache Thrift C++ library and headers

On Debian/Ubuntu-based systems, the required Thrift packages are commonly
available as `thrift-compiler` and `libthrift-dev`.

## Build

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
```

The build generates the Thrift C++ sources in `build/generated`. Generated
source files under `generated/` are kept only as a repository reference; the
CMake targets compile from the build directory.

## Run

Run the custom TCP server, then the corresponding client in a second terminal:

```sh
./build/mini_thriftme_server
./build/mini_thriftme_client
```

The custom server listens on port `8080` and the client requests
`GetVehicleSpeed`.

For the Apache Thrift implementation, run:

```sh
./build/thrift_server
./build/thrift_client
```

The Thrift server listens on port `9090`.

## Tests

```sh
ctest --test-dir build --output-on-failure
```

The unit test covers custom RPC request/response serialization, malformed wire
data rejection, and `ServiceBroker` dispatch behavior.

## Custom TCP protocol

The custom transport exchanges one request and one response per connection.
Messages are UTF-8 text with `|`-separated fixed fields; the payload may itself
contain `|` characters.

```text
request:  <request-id>|<method>|<payload>
response: <request-id>|<success: 0 or 1>|<payload>
```

After sending a request, the client closes its write side of the TCP connection.
The server reads the complete request, writes the response, and closes the
connection. Both sides limit a single message to 64 KiB.

Supported custom RPC methods are `GetVehicleSpeed`, `GetCpuUsage`, and
`GetMemoryUsage`.
