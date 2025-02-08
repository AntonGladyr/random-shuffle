# Build the application
FROM ubuntu:22.04 AS builder

# Install build tools and dependencies.
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
      build-essential \
      cmake \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

COPY CMakeLists.txt ./

# Then copy the rest of the source code.
COPY . .

# Create a separate build directory and compile the application.
RUN mkdir build && cd build && \
    cmake .. && \
    make -j"$(nproc)"


# Create the runtime image
FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
      libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy the built binary from the builder stage.
COPY --from=builder /src/build/shuffle.x /usr/local/bin/shuffle.x

RUN useradd -m appuser
USER appuser


ENTRYPOINT ["/usr/local/bin/shuffle.x"]
