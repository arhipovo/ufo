// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <assert.h>
#include <memory>

#include <chainparamsseeds.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "2 january 2014";
    const CScript genesisOutputScript = CScript() << 0x0 << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 400000;
        consensus.BIP34Height = 266000;
        consensus.BIP34Hash = uint256S("0xcfbf5f2e1cad950d5c36373be816aef9b3ad0cf2cdac1a9fa2547cf866be1865");
        consensus.BIP65Height = 1205150; // 36c9dce9da620d445dd628b302384d73a05d66655e020c69b9eee3481cccb8db
        consensus.BIP66Height = 1205150; // 36c9dce9da620d445dd628b302384d73a05d66655e020c69b9eee3481cccb8db
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.vAlertPubKey = ParseHex("04b48eaf546f46221b6b3ee0806f7652763ab5e9774125636ef539f144e98d176e02274600ed6b605cfcc199aba8f7d2228d2cc6b9b28d6fa244b74f7540c22c2a");
        consensus.checkpointPubKey = "044318157bd82b6e17926c8804eecf73140f416c34ccc2237c56614d081dce88a98293b40891d801d16a2899defe7869706d7ec55118ef8f06c683cfdc6b6a8c58";
        consensus.nPowTargetTimespan = 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 90;
        consensus.nCoinFix = 15000;
        consensus.nHardForkOne = 33479;
        consensus.nHardForkTwo = 160997;
        consensus.nHardForkTwoA = 171900;
        consensus.nHardForkThree = 266000;
        consensus.nHardForkFour = 1182000;
        consensus.nHardForkFourA = 1220000;
        consensus.nNeoScryptFork = 1414446393;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 10080; // 75%
        consensus.nMinerConfirmationWindow = 13440;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1517443200; // Feb 1st, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1548979200; // Feb 1st, 2019

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1519862400; // Mar 1st, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1551398400; // Mar 1st, 2019

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000022c5539b22f8d6");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0xe4d06a126e75abe493b1d07e3c2112a9121ba9e9d66ec82da53acd48196b05fc "); //1314322

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfc;
        pchMessageStart[1] = 0xd9;
        pchMessageStart[2] = 0xb7;
        pchMessageStart[3] = 0xdd;
        nDefaultPort = 9887;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1388681920, 1671824, 0x1e0ffff0, 1, 0);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xba1d39b4928ab03d813d952daf65fb7797fcf538a9c1b8274f4edc8557722d13"));
        assert(genesis.hashMerkleRoot == uint256S("0x8207df3a28a5bfdcaba0c810e540123aaea8d067b745092849787169f5e77065"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they dont support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.emplace_back("dns.seed1.ufocoin.net");
        vSeeds.emplace_back("dns.seed2.ufocoin.net");
        vSeeds.emplace_back("dns.dnsseed.lowecraft.it");
        vSeeds.emplace_back("dns.dnsseed.ufocoinnode.com");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,27);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,68);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,155);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "uf";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = {
            {
                {   4500, uint256S("0x5755857a8055c732d5236b0526afcb9b92f1291c87ed3c655c6d79df6b9d3dd4")},
                {   9999, uint256S("0x808bf9bdf3c7e777ad8008455f6849001bc264910de86e01a0bf1d83ed362aba")},
                {  20000, uint256S("0xe14a9e1d1cd79fa0385d3af7eac36ed96f29d7c0205b62eb82c4e7c5b043c6d1")},
                {  33349, uint256S("0xcf9ea4ab6589b0ac0cc34fca94ea3c24842ac80f43724d0c8d89ece0aa0a5081")},
                {1079136, uint256S("0xe171e30fa1ab3428f079a165a22f5cfd3529fb0e76bd0e7213a3ac9a09bd5571")},
                {1213947, uint256S("0xa4c7b570fbf1d755c327ff9c3d98e9d5433e453f9ecade20a8e4852bd124eb8f")},
                {1246467, uint256S("0xa0e2460c7e644cbb6c4bc01088094524fdf90892aa42d22f9dd9b6e3c981ca6f")},
                {1314322, uint256S("0xe4d06a126e75abe493b1d07e3c2112a9121ba9e9d66ec82da53acd48196b05fc ")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block e4d06a126e75abe493b1d07e3c2112a9121ba9e9d66ec82da53acd48196b05fc  (height 1314322).
            1526634445, // * UNIX timestamp of last known number of transactions
            1627450,  // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.001         // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 400000;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x45b4e55bddf20dfeb69ef2a35dd36f58dd45d5f4582c1a4ca1c1b78eef8f8c37");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.vAlertPubKey = ParseHex("0452c73ce2a53acd207b5c7f643c80d1bae3b13263b443762ef772de30c7fb7fcc3b7b4b1b19d025e730a0beb6245cacb668118e34a2b0fed2dd8c8fa44a8357d6");
        consensus.checkpointPubKey = "04d0dd87fbb6ac3483f57c9cd010c8fa804219ec641fad97a9cbb31605327b15fa9c40232fa214f02b80883955f7b14e49dbd03e44d45123f06ee08b911a08be33";
        consensus.nPowTargetTimespan = 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 90;
        consensus.nHardForkTwo = 1;
        consensus.nHardForkTwoA = 1;
        consensus.nHardForkThree = 1;
        consensus.nHardForkFour = 1100;
        consensus.nHardForkFourA = 1500;
        consensus.nNeoScryptFork = 1506816000;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 375; // 75%
        consensus.nMinerConfirmationWindow = 500;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1516924800; // Jan 26th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1548460800; // Jan 26th, 2019

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1519862400; // Mar 1st, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1551398400; // Mar 1st, 2019

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000100010");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x45b4e55bddf20dfeb69ef2a35dd36f58dd45d5f4582c1a4ca1c1b78eef8f8c37"); //1135275

        pchMessageStart[0] = 0xfb;
        pchMessageStart[1] = 0xc0;
        pchMessageStart[2] = 0xb8;
        pchMessageStart[3] = 0xdb;
        nDefaultPort = 19887;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1388678813, 616291, 0x1e0ffff0, 1, 0);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x45b4e55bddf20dfeb69ef2a35dd36f58dd45d5f4582c1a4ca1c1b78eef8f8c37"));
        assert(genesis.hashMerkleRoot == uint256S("0x8207df3a28a5bfdcaba0c810e540123aaea8d067b745092849787169f5e77065"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("testnet-seed.ufocoin.net");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,130);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "ut";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;


        checkpointData = {
            {
                {0, uint256S("45b4e55bddf20dfeb69ef2a35dd36f58dd45d5f4582c1a4ca1c1b78eef8f8c37")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 00000000000001c200b9790dc637d3bb141fe77d155b966ed775b17e109f7c6c (height 1156179)
            1388678813,
            1,
            0.001
        };

    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.vAlertPubKey = ParseHex("0452c73ce2a53acd207b5c7f643c80d1bae3b13263b443762ef772de30c7fb7fcc3b7b4b1b19d025e730a0beb6245cacb668118e34a2b0fed2dd8c8fa44a8357d6");
        consensus.checkpointPubKey = "04d0dd87fbb6ac3483f57c9cd010c8fa804219ec641fad97a9cbb31605327b15fa9c40232fa214f02b80883955f7b14e49dbd03e44d45123f06ee08b911a08be33";
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.nHardForkTwo = 1;
        consensus.nHardForkTwoA = 1;
        consensus.nHardForkThree = 1;
        consensus.nHardForkFour = 1;
        consensus.nHardForkFourA = 1;
        consensus.nNeoScryptFork = 1524473955;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x1b;
        pchMessageStart[1] = 0x21;
        pchMessageStart[2] = 0x55;
        pchMessageStart[3] = 0x1c;
        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1296688602, 3, 0x207fffff, 1, 0);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xa482cf37ea99d8c74f62e28903208bfbc12901b35738feff20fdf7e3b671afb7"));
        assert(genesis.hashMerkleRoot == uint256S("0x8207df3a28a5bfdcaba0c810e540123aaea8d067b745092849787169f5e77065"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = {
            {
                {0, uint256S("a482cf37ea99d8c74f62e28903208bfbc12901b35738feff20fdf7e3b671afb7")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,130);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "ufrt";
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
