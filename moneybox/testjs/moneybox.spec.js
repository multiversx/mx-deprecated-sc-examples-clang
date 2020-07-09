var assert = require("chai").assert;
var erdjs = require("@elrondnetwork/erdjs");
var erdtestjs = require("@elrondnetwork/erdtestjs");
var Address = erdjs.Address;
var World = erdtestjs.World;


describe("Array", function () {
    let aliceBech32 = "erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz";
    let bobBech32 = "erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r";
    let alice = new Address(aliceBech32);
    let bob = new Address(bobBech32);
    let GAS_LIMIT = 500000;

    it("should not accept money at deploy", async () => {
        let world = await setupWorld(this.ctx.test.title);

        let deployResponse = await world.deployContract({ impersonated: alice, code: getCode(), gasLimit: GAS_LIMIT, value: 5000 });
        assert.equal(deployResponse.Output.ReturnCode, 4);
        assert.equal(deployResponse.Output.ReturnMessage, "attempted to transfer funds via a non-payable function");
    });

    it("should be initialized after deploy", async () => {
        let world = await setupWorld(this.ctx.test.title);
        let contract = await setupContract(world);

        // Creator is stored
        let storage = world.getAccountStorage(contract);
        assert.isTrue(storage.byString("CREATOR_ADDRESS").asAddress().equals(alice));

        // No money yet (zero ERD)
        let queryResponse = await world.queryContract({ contract: contract, impersonated: alice, functionName: "getTotalAmount", gasLimit: GAS_LIMIT });
        assert.equal(queryResponse.firstResult().asNumber, 0);
    });

    it("should allow only the creator to deposit", async () => {
        
    });

    it("deposit should not accept more than 100 ERD", async () => {
        let world = await setupWorld(this.ctx.test.title);
        let contract = await setupContract(world);
    });

    it("should send money to address when target is reached", async () => {
        
    });

    async function setupWorld(name) {
        let world = new World(name);
        await world.createAccount({ address: alice, nonce: 42 });
        await world.createAccount({ address: bob, nonce: 42 });
        return world;
    }

    async function setupContract(world) {
        let deployResponse = await world.deployContract({ impersonated: alice, code: getCode(), gasLimit: GAS_LIMIT });
        assert.isTrue(deployResponse.isSuccess());

        let contract = deployResponse.getContractAddress();
        return contract;
    }

    function getCode() {
        return erdtestjs.loadContractCode("../output/moneybox.wasm");
    }
});