# Interaction

## On devnet

Deploy & interact with contract:

```
cd simple-counter

python3 ./interaction/playground.py --pem=./testnet/wallets/users/alice.pem --proxy=http://localhost:7950
```

Interact with existing contract:

```
cd simple-counter

python3 ./interaction/playground.py --pem=./testnet/wallets/users/alice.pem --proxy=http://localhost:7950 --contract=erd1...
```

## On testnet

Deploy & interact with contract:

```
cd simple-counter

python3 ./interaction/playground.py --pem=my.pem --proxy=https://testnet-gateway.elrond.com
```

Interact with existing contract:

```
cd simple-counter

python3 ./interaction/playground.py --pem=my.pem --proxy=https://testnet-gateway.elrond.com --contract=erd1...
```
