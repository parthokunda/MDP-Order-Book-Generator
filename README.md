# Order Book Generator CME Globex MDP 3.0 Data
C++ Program to generate order book by parsing CME MDP 3.0 Data (.pcap) file.

## Installation
Code tested on Ubuntu LTS 22.04.1
```sh
# Install necessary library
sudo apt install g++ libpcap-dev zstd
```
## Dataset
```sh
# Download Dataset 
wget https://sample-pcaps-dl.databento.com/glbx-all/20230716/dc3-glbx-a-20230716T133000.pcap.zst
# Decompress .zstd file
unzstd dc3-glbx-a-20230716T133000.pcap.zst
```
## Usage
```sh
# compile
g++ -std=c++17 main.cpp -o main.o -lpcap
# run file
./main.o dc3-glbx-a-20230716T133000.pcap
```
Follow Command Line prompts to generate orderbook for a particular security ID.

## Approach to Generate Orderbook
* Update the orderbook for a security if there is a Snapshot Full Refresh (Template ID: 52).
* Higher sending time means more updated book, and showing the most updated book in the end.