ALICE="${USERS}/alice.pem"
CONTRACT_ADDRESS="erd1YourContractHere"

deploy() {
    erdpy --verbose contract deploy --project=${PROJECT} --recall-nonce --pem=${ALICE} --gas-limit=5000000

increment() {
    erdpy --verbose contract call ${CONTRACT_ADDRESS} --recall-nonce --pem=${ALICE} --gas-limit=5000000 --function="increment"
}

decrement() {
    erdpy --verbose contract call ${CONTRACT_ADDRESS} --recall-nonce --pem=${ALICE} --gas-limit=5000000 --function="decrement"
}

get() {
    erdpy --verbose contract query ${CONTRACT_ADDRESS} --function="get"
}
