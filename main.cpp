#include <iostream>
#include "util/ParserUtilities.h"
#include "fstream"
#include "engine/GreedyIncrementalClusteringEngine.h"
#include "util/Function/peak/FractionTICPeakFunction.h"
#include "util/ClusterUtilities.h"



int main() {



//    cout << "test" << endl;
//    string test_cluster = "BEGIN CLUSTER Id=22706 Charge=2 ContainsPeaklist=true\nBEGIN IONS\nTITLE=id=PXD000316;PRIDE_Exp_Complete_Ac_29985.xml;spectrum=139\nPEPMASS=569.2871704101562\nCHARGE=2.0+\nSEQ=STKAQNPMR\nTAXONOMY=4932\nUSER02=SwissProt:Q3E757\n175.218\t6.757\n209.261\t1.065\n212.055\t1.615\n213.004\t2.493\n221.216\t1.355\n230.216\t1.406\n240.270\t1.002\n259.057\t2.307\n265.186\t2.267\n273.023\t1.230\n277.169\t1.675\n314.311\t3.357\n318.158\t1.399\n322.104\t1.861\n347.379\t1.354\n348.402\t2.782\n350.442\t3.065\n352.175\t1.171\n355.279\t3.069\n359.093\t1.626\n363.239\t3.573\n369.560\t4.243\n370.327\t9.850\n373.242\t1.442\n375.210\t2.488\n376.785\t3.175\n385.374\t3.868\n388.071\t46.316\n392.991\t1.644\n396.796\t34.374\n401.058\t4.642\n406.053\t18.548\n407.362\t6.339\n410.726\t1.852\n419.194\t182.605\n421.118\t5.898\n435.968\t2.797\n437.027\t7.512\n439.326\t3.019\n441.024\t9.153\n447.077\t2.846\n449.054\t1.482\n454.002\t29.399\n455.049\t8.772\n459.143\t11.783\n460.297\t4.998\n464.161\t3.386\n466.989\t2.699\n469.649\t4.568\n471.148\t6.434\n472.963\t6.475\n476.418\t23.710\n477.235\t81.378\n482.295\t2.116\n484.212\t2.770\n487.459\t3.418\n488.231\t7.134\n489.013\t3.451\n495.142\t4.098\n496.449\t16.850\n504.921\t115.574\n512.119\t6.267\n515.155\t110.034\n516.034\t28.157\n519.738\t9.775\n524.737\t4.341\n526.271\t7.775\n528.046\t5.110\n533.188\t167.922\n536.716\t17.425\n537.513\t31.791\n543.047\t24.742\n546.455\t19.497\n551.196\t335.725\n552.009\t167.355\n560.315\t283.051\n568.821\t5.603\n587.225\t8.654\n590.257\t9.117\n594.476\t5.141\n605.119\t32.814\n617.414\t2.193\n620.896\t2.289\n622.414\t1.488\n635.471\t1.722\n646.403\t3.318\n650.099\t1.256\n661.258\t74.083\n676.324\t2.038\n683.342\t6.440\n684.444\t3.766\n685.533\t2.272\n691.421\t2.429\n702.182\t35.849\n719.336\t130.646\n732.290\t74.468\n757.270\t1.660\n764.629\t3.557\n799.503\t2.411\n816.356\t3.831\n834.567\t4.901\n838.377\t1.866\n843.507\t2.739\n855.131\t1.998\n880.513\t1.750\n907.346\t4.848\n914.262\t3.124\n946.245\t7.659\n963.383\t26.006\n969.070\t1.719\n991.630\t8.864\n1098.353\t9.571\nEND IONS\nBEGIN IONS\nTITLE=id=PXD000316;PRIDE_Exp_Complete_Ac_29985.xml;spectrum=171\nPEPMASS=554.2818603515625\nCHARGE=2.0+\nSEQ=SAKAQNPMR\nTAXONOMY=4932\nUSER02=SwissProt:P0C0W9\n175.147\t21.350\n183.125\t8.343\n199.697\t3.147\n200.568\t3.724\n217.948\t11.484\n247.214\t6.088\n265.210\t6.607\n310.897\t4.273\n314.297\t3.108\n322.255\t7.020\n340.167\t4.840\n345.933\t2.488\n358.119\t47.006\n370.168\t5.069\n373.378\t3.570\n375.449\t21.039\n376.242\t90.419\n381.677\t3.726\n387.239\t12.372\n388.355\t5.505\n393.129\t5.127\n401.210\t12.590\n402.364\t3.171\n405.469\t4.056\n410.687\t3.662\n413.762\t7.840\n419.217\t432.852\n426.151\t3.173\n428.646\t13.413\n429.339\t28.501\n433.600\t3.250\n438.552\t9.401\n440.392\t7.096\n447.201\t205.972\n454.384\t82.888\n460.841\t8.290\n461.863\t3.138\n464.256\t3.409\n468.877\t5.325\n470.247\t3.348\n473.180\t3.455\n481.376\t19.815\n489.708\t139.018\n490.479\t67.893\n494.161\t12.601\n495.858\t8.954\n501.397\t14.795\n503.637\t15.812\n505.392\t7.513\n508.849\t5.881\n521.587\t19.647\n522.432\t54.259\n533.266\t388.226\n535.409\t77.727\n536.500\t282.404\n539.665\t20.520\n545.636\t818.571\n557.153\t10.193\n560.240\t10.948\n574.509\t9.900\n575.130\t44.609\n608.045\t2.713\n616.320\t3.100\n631.032\t3.201\n643.473\t10.618\n644.279\t5.936\n661.273\t185.431\n668.489\t6.764\n671.261\t16.488\n672.268\t63.220\n688.802\t46.352\n689.416\t230.713\n700.555\t9.229\n715.227\t26.099\n719.349\t6.552\n732.320\t255.615\n775.451\t3.225\n786.548\t5.761\n858.817\t3.128\n907.532\t7.638\n916.193\t12.199\n933.307\t51.850\n1069.639\t3.524\nEND IONS\nBEGIN IONS\nTITLE=id=PXD000316;PRIDE_Exp_Complete_Ac_29985.xml;spectrum=195\nPEPMASS=526.2453002929688\nCHARGE=2.0+\nSEQ=MLQQGSSSR\nTAXONOMY=4932\nUSER02=SwissProt:P40095\n162.128\t6.814\n175.088\t19.423\n190.077\t254.050\n242.100\t47.319\n244.197\t8.203\n257.181\t5.935\n262.243\t136.816\n274.288\t5.205\n275.117\t7.192\n303.109\t67.721\n313.165\t13.870\n320.188\t9.730\n331.212\t7.279\n349.219\t322.329\n353.316\t7.552\n357.219\t8.246\n365.127\t19.396\n366.085\t6.690\n366.833\t21.620\n370.129\t42.255\n371.342\t10.050\n375.306\t59.374\n399.938\t17.098\n401.528\t10.229\n402.269\t5.165\n411.899\t10.320\n413.125\t14.746\n413.892\t19.342\n423.067\t82.307\n431.625\t833.286\n436.218\t128.859\n442.292\t5.699\n447.772\t7.325\n457.238\t19.998\n461.826\t8.274\n466.279\t10.948\n472.665\t11.073\n475.298\t19.528\n478.901\t12.738\n485.100\t25.199\n487.332\t11.639\n488.752\t22.895\n493.294\t527.999\n496.845\t16.846\n499.351\t34.098\n508.368\t916.133\n517.387\t972.687\n542.187\t7.317\n559.133\t254.092\n585.471\t11.298\n586.613\t5.618\n603.298\t43.201\n604.249\t15.523\n616.268\t158.024\n621.278\t639.677\n675.060\t20.880\n685.225\t66.761\n703.233\t306.453\n731.331\t16.048\n732.337\t30.974\n749.349\t276.516\n754.174\t25.957\n772.128\t74.646\n789.978\t47.536\n826.540\t24.635\n827.756\t10.873\n845.391\t80.630\n862.442\t883.537\n877.223\t17.377\nEND IONS\nBEGIN IONS\nTITLE=id=PXD000316;PRIDE_Exp_Complete_Ac_29985.xml;spectrum=317\nPEPMASS=594.2498168945312\nCHARGE=2.0+\nSEQ=MSANISETTAM\nTAXONOMY=4932\nUSER02=SwissProt:P06169\n175.076\t2.658\n206.189\t1.719\n219.039\t4.939\n221.118\t4.543\n237.111\t9.027\n246.270\t10.090\n247.160\t1.567\n258.143\t13.794\n259.325\t1.544\n267.244\t4.796\n281.057\t11.069\n286.096\t1.554\n288.243\t2.472\n291.089\t17.048\n293.273\t1.544\n299.228\t2.116\n300.276\t3.812\n314.277\t3.470\n322.165\t11.197\n326.387\t2.477\n338.019\t10.088\n340.205\t3.037\n343.164\t1.803\n344.066\t2.669\n347.209\t2.172\n355.069\t209.970\n355.977\t10.004\n370.984\t30.562\n372.164\t5.646\n376.147\t5.174\n382.975\t1.439\n392.246\t1.836\n403.327\t2.430\n404.894\t1.478\n417.068\t1.837\n418.368\t2.271\n420.214\t5.822\n421.190\t2.111\n424.340\t3.702\n428.303\t3.245\n430.678\t2.234\n435.173\t2.689\n439.106\t94.379\n448.260\t3.317\n451.813\t4.722\n458.158\t3.048\n459.426\t3.034\n463.042\t16.615\n464.238\t3.843\n468.053\t4.547\n470.100\t2.869\n473.006\t3.283\n478.335\t9.358\n479.260\t4.334\n480.105\t10.125\n484.848\t5.205\n487.997\t2.664\n490.261\t6.704\n494.844\t4.620\n504.210\t6.352\n510.398\t4.787\n511.638\t7.756\n513.273\t4.263\n516.139\t4.132\n517.132\t8.969\n518.407\t4.490\n522.554\t8.602\n528.288\t237.567\n530.436\t25.977\n533.136\t199.491\n537.089\t6.420\n539.333\t15.281\n541.259\t7.066\n544.604\t17.770\n550.875\t8.290\n553.024\t10.648\n554.578\t11.541\n556.800\t13.066\n557.892\t22.923\n562.507\t24.082\n568.197\t27.467\n576.246\t190.732\n577.050\t55.162\n583.616\t54.201\n585.413\t110.438\n594.619\t4.371\n598.646\t2.028\n602.262\t3.473\n620.202\t26.839\n631.452\t4.761\n637.293\t16.218\n643.530\t1.744\n655.143\t189.472\n660.706\t2.385\n684.039\t1.903\n690.923\t2.768\n700.503\t3.407\n731.303\t10.325\n732.340\t20.262\n749.244\t117.076\n768.526\t6.803\n778.644\t2.051\n799.980\t1.754\n817.314\t1.867\n832.351\t6.246\n834.507\t2.991\n850.293\t16.743\n875.012\t1.828\n900.295\t4.394\n923.261\t2.509\n933.505\t14.087\n951.320\t30.790\n1004.463\t24.844\n1022.292\t74.318\nEND IONS\nEND CLUSTER\n";
//    vector<ICluster *> a = ParserUtilities::readSpectralCluster(test_cluster);
//    cout << "end" << endl;

//    ifstream fin("/Users/sangzhe/ClionProjects/CSpectra-cluster/spectra_400.0_4.0.mgf");
//    ifstream fin2("/Users/sangzhe/ClionProjects/CSpectra-cluster/cluster_spec_4776.mgf");
//    ifstream fin3("/Users/sangzhe/ClionProjects/CSpectra-cluster/small.mgf");

    ifstream fin4("/Users/sangzhe/795.mgf");

    stringstream ss;
    ss << fin4.rdbuf();
    PointerPool* pointer_pool = PoolFactory::getInstance();


    time_t time_1,time_2,time_3,time_4;
    time_1 = time(NULL);

    vector<ISpectrum*> testSpectra = ParserUtilities::readMGFScan(ss);

    time_2 = time(NULL);
    cout << "testSpectra:"<<testSpectra.size() <<endl;

    cout << time_2 - time_1 <<endl;

    sort(testSpectra.begin(),testSpectra.end(),Spectrum::cmpSpectrumMZ);


    vector<ICluster*> Cluster;
    vector<ICluster*> secondCluster;


    GreedyIncrementalClusteringEngine *engine = new GreedyIncrementalClusteringEngine(new CombinedFisherIntensityTest(0.5F),4, 0.6, new FractionTICPeakFunction(0.5F, 20));
    GreedyIncrementalClusteringEngine *secondEngine = new GreedyIncrementalClusteringEngine(new CombinedFisherIntensityTest(0.5F),4, 0.95, new FractionTICPeakFunction(0.5F, 20));
    time_3 = time(NULL);

    for (ISpectrum* s:testSpectra) {
//        ICluster *spectrumCluster1 = ClusterUtilities::asCluster(s);
//
//        // use engine
//        vector<ICluster*> removedClusters = engine->addClusterIncremental(spectrumCluster1);
//
//        cout<< "num of clusters: "<< engine->getClusters().size() << endl;
//        for(ICluster* cls:removedClusters){
//            Cluster.push_back(cls);
//        }
//         use second engine
        ICluster *spectrumCluster2 = ClusterUtilities::asCluster(s);
        vector<ICluster*> secondRemovedClusters = secondEngine->addClusterIncremental(spectrumCluster2);
//
//        for(ICluster* cls:secondRemovedClusters){
//            secondCluster.push_back(cls);
//        }
//        for(vector<ICluster*>::iterator i = secondRemovedClusters.begin();i!= secondRemovedClusters.end();++i){
//            secondCluster.push_back(*i);
//        }
    }
    time_4 = time(NULL);
    cout<< "clusters:"<<secondEngine->getClusters().size() << endl;

    cout<< time_4 - time_3 << "s"<< endl;



//    GreedySpectralCluster *spectralCluster = new GreedySpectralCluster("testId");
//
//    spectralCluster->addSpectra(testSpectra);
//    xxx = spectralCluster->getConsensusSpectrum();
//    ISpectrum* yy = spectralCluster->getConsensusSpectrum();


    int s = pointer_pool->size();


}




