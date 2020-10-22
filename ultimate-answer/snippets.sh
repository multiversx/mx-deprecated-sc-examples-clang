ALICE="${USERS}/alice.pem"
CONTRACT_ADDRESS="erd1YourContractHere"

deploy() {
    erdpy --verbose contract deploy --project=${PROJECT} --recall-nonce --pem=${ALICE} --gas-limit=5000000
}

getUltimateAnswer() {
    erdpy --verbose contract query ${CONTRACT_ADDRESS} --function="getUltimateAnswer"
}
