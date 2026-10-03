FROM node:20-bookworm

RUN apt-get update && apt-get install -y g++ make && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY api/package*.json ./api/
RUN cd api && npm ci --omit=dev

COPY . .

RUN g++ -std=c++17 -O2 \
main.cpp \
core/*.cpp \
routing/*.cpp \
simulation/*.cpp \
-o netsim

WORKDIR /app

EXPOSE 5000

CMD ["node","api/server.js"]