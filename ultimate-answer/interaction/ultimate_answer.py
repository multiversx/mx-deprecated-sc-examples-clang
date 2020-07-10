import logging
from argparse import ArgumentParser

from erdpy import config
from erdpy.proxy import ElrondProxy
from erdpy.accounts import Account
from erdpy.contracts import SmartContract
from erdpy.environments import TestnetEnvironment
from erdpy.projects import ProjectClang

logger = logging.getLogger("examples")


if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument("--proxy", help="Testnet Proxy URL", default=config.get_proxy())
    parser.add_argument("--contract", help="Existing contract address")
    parser.add_argument("--pem", help="PEM file", required=True)
    args = parser.parse_args()

    logging.basicConfig(level=logging.DEBUG)

    # Create a project object
    project = ProjectClang("../")

    # We can inspect the bytecode like this:
    bytecode = project.get_bytecode()
    logger.info("Bytecode: %s", bytecode)

    # Now, we create a environment which intermediates deployment and execution
    environment = TestnetEnvironment(args.proxy)
    user = Account(pem_file=args.pem)

    # We initialize the smart contract with an actual address if IF was previously deployed,
    # so that we can start to interact with it ("query_flow")
    contract = SmartContract(address=args.contract)

    # A flow defines the desired steps to interact with the contract.
    def deploy_flow():
        global contract

        # For deploy, we initialize the smart contract with the compiled bytecode
        contract = SmartContract(bytecode=bytecode)
        user.sync_nonce(ElrondProxy(args.proxy))

        tx, address = environment.deploy_contract(
            contract=contract, 
            owner=user, 
            arguments=[], 
            gas_price=config.DEFAULT_GAS_PRICE,
            gas_limit=500000,
            value=None,
            chain=config.get_chain_id(),
            version=config.get_tx_version()
        )
        logger.info("Tx hash: %s", tx)
        logger.info("Contract address (hex): %s", address.hex())
        logger.info("Contract address (bech32): %s", address.bech32())

    def query_flow():
        global contract

        answer = environment.query_contract(contract, "getUltimateAnswer")
        logger.info(f"Answer: {answer}")

    while True:
        print("Let's run a flow.")
        print("1. Deploy smart contract")
        print("2. Query smart contract")

        try:
            choice = int(input("Choose:\n"))
        except Exception:
            break

        if choice == 1:
            environment.run_flow(deploy_flow)
        elif choice == 2:
            environment.run_flow(query_flow)
