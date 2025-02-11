FROM debian:bookworm-slim AS build

# Install build tools and dependencies.
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
      build-essential \
      cmake \
      libomp-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

# Copy the build configuration first to take advantage of Docker’s cache.
# (If you have additional dependency files, copy them here.)
COPY CMakeLists.txt ./

COPY . .

# Create a separate bin directory and compile the application.
RUN cmake -S . -B bin -DCMAKE_BUILD_TYPE=Release && \
    cmake --build bin -- -j"$(nproc)"


# Create the runtime image
FROM debian:bookworm-slim

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
      libstdc++6 \
      libgomp1 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy the built binary from the build stage.
COPY --from=build /src/bin/shuffle /usr/local/bin/shuffle

RUN useradd -m appuser
USER appuser


ENTRYPOINT ["/usr/local/bin/shuffle"]
