# Dockerfile
FROM ubuntu:22.04

# Prevent interactive prompts during apt install
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC



# Update package list and install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libssl-dev \
    libpq-dev \
    libmysqlclient-dev \
    libboost-all-dev \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /tmp

# Clone Wt repository
RUN git clone --depth 1 https://github.com/emweb/wt.git

# Create build directory and configure build
WORKDIR /tmp/wt
RUN mkdir build && cd build

# Configure with cmake
WORKDIR /tmp/wt/build

RUN cmake .. \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    -DCONNECTOR_HTTP=ON \
    -DCONNECTOR_FCGI=OFF \
    -DWT_CPP_11_MODE=-std=c++14 \
    -DCMAKE_BUILD_TYPE=Release \
    -DENABLE_SSL=ON \
    -DBUILD_EXAMPLES=OFF \
    -DBUILD_TESTS=OFF

# Compile Wt (using all available cores)
RUN make -j$(nproc)

# Install Wt
RUN make install

# Update library cache
RUN echo "/usr/local/lib" >> /etc/ld.so.conf.d/wt.conf && ldconfig

# Create Wt configuration directory and file
RUN mkdir -p /wt_custom_startup && touch /wt_custom_startup/wt_config.xml

# Create a basic wt_config.xml file
RUN echo '<server><application-settings location="*"><session-management><multi-threaded/></session-management><theme>bootstrap5</theme><progressive-bootstrap>true</progressive-bootstrap><ajax-puzzle>true</ajax-puzzle><debug>false</debug><log-file></log-file><log-config></log-config><max-request-size>128</max-request-size><session-id-length>16</session-id-length></application-settings></server>' >> /wt_custom_startup/wt_config.xml

# Set working directory for your application
WORKDIR /app

# Copy source code
COPY . .

# Compile the application
RUN g++ -std=c++14 -o webapp _main.cc -lwt -lwthttp

# Expose port
EXPOSE $PORT

# Run the application (PORT will be provided by Host)
CMD ["sh", "-c", "./webapp --docroot=. --http-address=0.0.0.0 --http-port=$PORT --config=/wt_custom_startup/wt_config.xml"]
