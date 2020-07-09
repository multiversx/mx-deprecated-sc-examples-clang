DENOMINATION="000000000000000000"
PROXY="https://api.elrond.com"
CHAIN="BoN"
ALICE_PEM="~/wallet/alice.pem"
CONTRACT_ADDRESS="erd1YourContractHere"

deploy() {
    erdpy --verbose contract deploy ${CONTRACT_FOLDER} --recall-nonce --pem=${ALICE_PEM} --gas-limit=5000000 --proxy=${PROXY} --chain=${CHAIN}
}

getUltimateAnswer() {
    erdpy --verbose contract query ${CONTRACT_ADDRESS} --function="getUltimateAnswer" --proxy=${PROXY}
}
