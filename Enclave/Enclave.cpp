/*
 * Copyright (C) 2011-2018 Intel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#include <stdarg.h>
#include <stdio.h>      /* vsnprintf */
#include <vector>

#include "Enclave.h"
#include "Enclave_t.h"  /* print_string */

/* 
 * printf: 
 *   Invokes OCALL to display the enclave buffer to the terminal.
 */

/*
void printf(const char *fmt, ...)
{
    char buf[BUFSIZ] = {'\0'};
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);
    ocall_print_string(buf);
}*/


float fm_predict(const float* order_1, const float* order_2, const size_t RANK,  const float bias, const std::vector<int>* nzi) {
    float rating = bias;

    int n = (*nzi).size(); // number of non-zero indices
    int i, j = 0;
    for (i = 0; i < n; i++) {
        int index_1 = (*nzi).at(i);
        rating += order_1[index_1];
        for (j = i+1; j < n; j++) {
            int index_2 = (*nzi)[j];
            float inner_product = 0.0;
            int r = 0;
            for (r = 0; r < RANK; r++) {
                inner_product += order_2[index_1 * RANK + r] * order_2[index_2 * RANK + r];
            }
            rating += inner_product;
        }
    }
    //println!("{}", rating);

    return rating;
}

size_t recommend(int gender, int age, int occupation){

    const size_t N = 98;
    const size_t RANK = 10;
    
    const float order_1[N] = {0.56478995, 0.46205544, 0.28694332, 0.49747017, 0.51103455, 0.5223141, 0.44959915, 0.44401088, 0.43065724, 0.48531565, 0.42191583, 0.4299466, 0.6297237, 0.5154706, 0.5299493, 0.63379747, 0.529883, 0.68948716, 0.42423716, 0.24681209, 0.41555065, 0.5036003, 0.62471014, 0.54887116, 0.49714482, 0.4895509, 0.5284425, 0.49500057, 0.45741677, 0.3576906, 0.08829016, 0.151813, 0.49524885, 0.35748908, 0.4000717, 0.56627756, 0.13920203, 0.478227, -0.21674198, -2.1548942e-05, -0.6374973, -0.98981917, 0.0005901663, 0.45932, 0.6000017, 0.5712341, 0.60650694, 0.0004318657, 0.77831423, -0.21599638, 0.045938555, -0.6488614, 0.5480866, 0.43407643, 0.37475693, 0.33359307, 0.7872764, 0.26880515, 0.2196455, -0.6373316, 0.46501732, 0.71791136, -1.1961306, 0.6876853, 0.93894, 0.36391136, -0.16400172, 0.739036, -0.08774931, -0.08925613, 0.24048871, 0.3225417, 0.75594366, 1.0140227, 0.34940398, 1.1908517, 1.062483, 0.9608546, 0.5651832, 0.564614, 0.39296862, 0.36525214, 0.61097515, 1.2766544, 0.13935843, 0.09163371, 0.87720674, 1.2776903, 0.60725033, -0.64346623, 0.76603925, 0.13116904, 0.40383303, 0.25435567, 0.602414, -0.9912436, 1.257431, 0.8361706};

    const float order_2[N][RANK] = {-0.066303894, -0.00097519736, -0.2117099, 0.38885126, 0.01909747, 0.11971418, 0.12489869, -0.004058837, -0.10859302, 0.0612681, 0.00016646039, -0.026755054, 0.11775604, -0.046814322, -0.008331671, -0.047671743, 0.11492486, -0.009408328, 0.04609172, 0.0020988637, -0.12414866, 0.21059908, 0.01912324, 0.20169757, -0.049853988, 0.19852072, -0.36215734, -0.1001726, 0.023987843, 0.14137615, -0.10246104, 0.04862697, 0.12518147, -0.14369947, 0.027543746, 0.031475447, -0.026734693, -0.12806666, 0.2452386, 0.14458616, -0.010889416, -0.037230056, 0.011267185, 0.00090000266, 0.050425142, -0.059176307, 0.013294197, -0.016007414, 0.118307374, 0.033890102, -0.033547733, -0.0728019, 0.18248408, 0.054273218, -0.05080437, 0.13835259, -0.1702796, -0.009825371, -0.016663035, -0.14853819, 0.066849425, -0.060405765, 0.13092376, 0.21743073, -0.112720616, -0.07963281, -0.06267832, 0.2626336, -0.01688787, -0.10008074, 0.11193686, -0.05743378, 0.086956665, 0.33292732, -0.1562883, -0.06018295, -0.1513871, 0.24688073, 0.061866198, -0.020938583, 0.32503018, 0.061150048, 0.11184434, 0.39924926, -0.1674268, -0.13315703, -0.36543617, 0.030435704, 0.043148234, -0.06458923, -0.10701682, 0.08606558, 0.09263607, 0.14697705, 0.00045661483, 0.08301541, -0.012702132, -0.16913578, 0.047061507, -0.055390272, 0.006632811, -0.26356146, 0.18137549, 0.11575141, 0.0068922103, 0.03963634, -0.00405033, 0.07190735, 0.12663688, -0.020607889, 0.0019096194, -0.037928417, 0.08731019, 0.10781997, 0.048005056, -0.16159481, -0.24741045, 0.13729808, 0.17774832, -0.24900967, 0.09086234, -0.28430918, 0.29848352, 0.046029836, -0.020429293, 0.1796532, -0.013440857, -0.039835583, 0.007984702, 0.009184984, 0.08423203, 0.13025752, 0.04946484, 0.11549874, -0.0512268, -0.113995746, -0.042822715, -0.009275381, 0.10127126, 0.11856236, 0.08589121, -0.013038451, -0.00060461985, 0.15342493, -0.0670109, -0.067154475, -0.14266656, -0.047729407, 0.22232617, 0.14782669, -0.15100242, 0.21125361, 0.37373173, 0.076524824, 0.057517737, 0.09020127, 0.17844272, -0.102496706, -0.08008412, 0.048934206, -0.1177314, -0.02755469, -0.032296125, 0.032679405, 0.047455266, -0.009136865, -0.10037141, -0.012379434, 0.10964138, -0.07199909, 0.331939, -0.3175634, 0.883572, -0.8616653, -0.118465744, -0.9377446, -0.024974428, 0.5274074, -0.16414024, -0.13025294, 0.026579585, 0.27320203, 0.17896833, 0.49964222, -0.05340281, -0.10245006, 0.5048704, -0.16281582, 0.15833516, 0.22041032, -0.18801868, 0.41948575, 0.050189734, 0.03734591, -0.043187413, 0.44236356, -0.38143647, -0.18832745, 0.18125184, 0.21672477, 0.3858191, -0.057268526, 0.13628143, -0.1928878, 0.024059467, -0.115761526, -0.19822337, 0.13351415, 0.04165929, 0.3169505, -0.18927403, -0.22966161, -0.23575592, -0.025441, -0.03469066, 0.11981173, -0.07942545, 0.0034503806, 0.2415831, 0.062138572, 0.013057305, -0.12848853, -0.28143528, 0.0121730035, -0.009676941, -0.2586973, 0.36775187, 0.08801157, 0.25988254, 0.5147559, 0.010256771, 0.077420704, 0.016538665, 0.1206101, 0.017228492, -0.16677058, -0.008043007, 0.0754934, 0.25776002, 0.04047971, -0.124139786, -0.0756012, 0.14123508, 0.08506626, 0.062864736, -0.039129995, 0.039271507, 0.09055233, 0.16791196, -0.116529964, -0.03722754, 0.034343366, 0.32357028, 0.046967387, 0.009114124, 0.011812087, 0.028807562, 0.031937305, -0.15651152, -0.036740977, 0.00715037, 0.024037182, -0.036410585, 0.22339757, -0.036684513, 0.1399252, -0.14927055, -0.0373706, 0.11743461, 0.030969968, -0.09090747, -0.040348485, 0.15516277, 0.02735872, -0.08602484, 0.2692348, 0.064179935, -0.11302814, 0.11933453, 0.100889444, -0.052442107, 0.07828221, 0.49376103, -0.18086152, 0.033702582, 0.08149421, -0.3954893, 0.20539561, -0.022714177, -0.11073772, 0.18942676, -0.3905267, 0.03409629, -0.07183214, -0.026248515, -0.13394274, -0.26460743, 0.1364735, 0.09650522, -0.042074088, 0.17092337, 0.251728, -0.057181537, -0.11449215, -0.22372004, 0.20190202, 0.04056853, 0.1645061, 0.22633898, -0.18617406, -0.032318134, -0.007560191, -0.19270356, -0.108738735, -0.100714624, 0.12818941, -0.10075653, 0.08334964, 0.21425603, -0.038624752, 0.036511496, -0.012703096, -0.0672493, -0.22028472, -0.047193095, 0.059927337, 0.06570781, -0.022372866, -0.33266035, -0.060833074, 0.034492917, -0.028851846, 0.11183294, 0.09127221, -0.08889653, 0.09001447, 0.03164366, 0.061348222, -0.06647975, -0.1304633, 0.11524255, 0.08892424, -0.13110587, -0.07709131, -0.13029106, 0.17946844, 0.111796245, 0.116569676, 0.052983843, -0.13276666, -0.104659185, 0.1380121, 0.073739685, -0.079990275, 0.064987846, -0.07751837, -0.07222739, -0.06794266, 0.13422465, 0.044293668, 0.12583834, -0.17585182, -0.99361223, -0.5639786, 0.08304709, 0.215205, 0.08373773, 0.061184205, -0.16250548, -0.4606582, 0.06040205, 0.0025667658, 0.007869779, -0.087996304, -0.03572809, 0.006938741, -0.063895814, 0.06866524, -0.088311285, -0.0035993014, 0.16519661, 0.2913884, 0.20985661, 0.23502839, -0.21137094, 0.1423931, -0.14128383, 0.15335554, 0.4792585, -0.41242966, -0.0006641331, 0.0006919935, 0.00018553424, -0.0009829226, -0.0009317103, 9.167078e-05, 0.0007622576, -0.0008716169, -0.00053332304, -0.00033897976, 0.054916628, 0.19262365, 0.22774881, 0.7977061, -0.2562422, 0.81637925, -0.04275226, 0.06586701, 0.745254, 0.14373015, 0.28466251, 0.2956394, 0.46310088, 0.4580054, -0.3888664, 0.41400787, 0.015049203, 0.060727343, 0.6843228, -0.34294096, 8.384918e-05, -0.00029262283, -0.0004573114, 0.0007453008, 0.0003544488, -0.00024160289, -0.000884095, 0.0006001709, -0.0007438257, 0.00013183162, 0.028474845, 0.0054987483, -0.04249665, 0.16695754, -0.02845662, -0.06536634, -0.012888906, 0.03432627, -0.10334498, 0.041360226, -0.032026824, -0.15299058, -0.016096072, -0.14959535, 0.16838366, -0.09733704, -0.18810414, -0.027918212, 0.29166678, 0.14024585, -0.02972462, 0.18849641, -0.0168303, -0.091884606, 0.008525083, -0.000909845, 0.01057374, -0.048688747, 0.027995333, -0.0019392505, 0.3173797, 0.06696461, -0.017924415, 0.07913523, -0.06343201, -0.32431334, 0.14356181, -0.23992674, -0.11550717, 0.17049535, 0.00044637255, 0.00014987204, 0.0009495872, -0.00021346047, 0.0009173341, -4.7739246e-05, 0.0005897081, 0.00037263345, -0.00015082047, -0.00022305682, 0.056147017, -0.0070317173, -0.309847, -0.37342343, -0.04396201, 0.27883777, 0.20214853, 0.05746537, -0.24381968, -0.07149122, 0.16530535, -0.1379314, 0.051126588, 0.23638448, -0.21124567, 0.14235343, -0.14126141, 0.15316395, 0.48302546, -0.017781144, 0.15702935, 0.4071987, -0.33046234, -0.24925134, -0.32961264, 0.65283024, 0.023107123, 0.17262653, 0.49689266, -0.17293781, 0.28069955, 0.8449059, -0.6477463, 0.7569737, -0.57567084, 0.7087517, 0.3569492, 0.33583474, 0.12155436, -0.8693064, -0.43279958, -0.17993417, 0.14634638, 0.7918171, 0.1786847, 0.23939408, -0.41010597, -0.5156236, -0.0199477, -0.1598299, 0.09709523, 0.48841268, -0.1287117, -0.33392808, -0.09176937, 0.0975799, 0.45310053, 0.09147775, -0.33716795, -0.13012394, -0.047287136, 0.122931704, -0.60156643, 0.1089742, -0.19033292, 0.3247905, 0.5447733, 0.026121702, 0.18700472, 0.017496502, 0.284398, -0.064492136, -0.1951834, 0.685015, -0.28174618, 0.6094416, 0.043437477, 0.31833068, -0.522239, -0.476847, -0.27001837, -0.70800835, -0.2803079, -0.55894774, 0.088551, -0.077157095, -1.6324742, 0.103212796, 0.55212116, 0.07740734, 0.20129213, 0.46758488, -0.23262131, -0.16058587, -0.26759642, 0.31305203, 0.053843986, 0.1824299, -0.27848545, -0.2449402, 0.40078214, 0.4068821, -0.39006573, -0.18082038, -0.40097415, 0.35221204, 0.46105054, 0.38593096, 0.082268104, -0.33286393, 0.052835993, 0.2454534, -0.3881149, -0.029354226, -0.2563812, 0.2055848, -0.9758898, 0.065487735, 0.6301644, -0.5372733, 0.33369517, 0.01443371, -0.13067836, 0.9498916, -0.17204757, -0.2800662, -0.17126478, 0.44550526, -0.08310857, -0.39071512, -0.51071095, -0.8350723, 1.2442642, 1.0119644, 0.7877625, -0.88967097, -1.7485785, -0.779695, -0.3855919, 0.5869481, 0.5161505, 0.3005253, 0.15549381, 0.70167035, -0.633789, 0.61447686, -0.18572436, 0.5976002, 0.21468407, -0.58706224, -0.081715025, 0.087171376, 0.041532602, -0.08260385, 0.2081582, -0.10967944, -0.05712952, -0.4801846, 0.48764148, 0.3954926, -0.32148346, -0.6339731, -0.42044386, 0.34555924, 0.41092256, -0.518401, -0.044449445, 0.01464686, -0.5042629, 0.45427343, -0.62274504, -0.35108176, -0.17689417, 0.16047986, 0.51799214, -0.8835725, 0.29016858, -0.4090377, 1.2604249, -0.16307905, -0.15218826, 0.39041513, -0.2123675, -0.33677718, -0.121711016, -0.081329495, -0.24671139, -0.39577818, 1.3380951, -0.060139097, 0.25621513, -0.37590858, -0.035318088, 0.03661979, -0.0712827, -0.18077163, 0.4038723, 0.14638951, -0.43649176, 0.09977662, 0.20699514, 0.24887258, -0.27024117, -0.08502618, -0.2595514, 0.1584513, -0.047326222, 0.18421683, 0.29144177, -0.47370037, 0.68210745, 0.8935727, -0.5105729, -0.21808699, -0.58474976, 0.58972764, 0.48724857, 0.4577904, -0.08714508, -0.46863753, -0.0140470825, 0.17275278, -0.5301873, 0.12732093, -0.3130651, 0.041406944, 0.24020173, 0.2753041, 0.8340277, -0.7017545, 0.5585822, 0.3053447, -0.3528727, -0.3161922, -0.6330702, 0.27525955, 0.2299177, 0.5469544, 0.2892484, -0.40226376, -0.07668671, -0.042261627, 0.5002792, -0.3456565, 0.2695714, -0.3132712, -0.20810634, -0.08721696, 0.4479483, 0.32654026, -0.4092722, -0.55815244, 0.5199367, 0.5656679, 0.66929275, -1.0577381, -0.79448605, -0.8806256, -0.97438544, 0.6180768, -0.6435045, 0.13637939, 1.0300146, -0.39975426, 0.21866077, 1.1244409, 0.5647988, -1.3010535, 0.15584646, 0.5931917, -1.0140471, -0.9724582, 0.7598988, 0.4405929, 1.1377872, -0.90413254, -0.52748555, -1.1038196, -1.1951754, 0.98079896, -0.7773168, -0.6898348, 0.11283107, 0.1474213, 0.8786323, -0.7883842, -0.13237245, -0.62226045, -0.40763336, 0.8235725, -1.0109321, -1.582359, -0.0824004, 0.7565894, 1.1735054, -0.8675996, -1.0489295, -0.73995304, -0.135169, 0.542525, 0.018681996, 0.03484601, 0.24676351, 0.74886787, 0.19605249, -0.6814548, 0.1406266, -0.3316178, 0.11733299, 0.13788009, -0.24446689, -0.13878638, -0.11218192, 0.067539945, 0.113479964, 0.005326446, -0.32103667, -0.252371, 0.39418834, 0.11936932, 0.026297256, 0.12460245, 0.0059957593, 0.14546914, -0.09934353, 0.08987724, 0.011292256, 0.016298123, 0.13527054, -0.12602894, 0.3695839, 0.006038468, -0.27781266, -0.1205675, -0.13019702, 0.11001127, 0.052783534, 0.38125944, 0.08569881, -0.49120694, 0.049439963, 0.4678195, -0.03152065, -0.18344721, -0.24938656, 0.106809825, 0.48986334, 0.09384095, 0.35141644, 0.12033415, -0.984005, -1.0378268, 0.95099235, 0.8325931, 1.1659328, -1.1009787, -1.2464317, -1.0400032, -0.84604836, 1.2431666, 0.10662035, -0.46679786, -0.9744154, -0.52592754, 0.088652626, 0.20578903, 0.10411352, 0.030138232, -1.6829343, -0.4952981, -0.09952902, -0.27280524, -0.6365088, 1.041696, -0.23017827, 0.55346525, -0.488781, -0.32107097, -0.46277815, -0.68677276, 0.06034094, -0.1397436, -0.21988808, 0.008074073, 0.07722749, 0.036739264, 0.18866558, 0.05817131, -0.29934576, 0.096042074, -1.1003083, -0.32399827, -0.2976164, 1.8108675, 1.174657, -1.2628539, -0.87955964, -1.0626556, -0.19904085, 1.1948665, 0.31689999, 0.031030897, 0.078686886, 0.07912754, -0.04970705, -0.32354954, 0.14441513, -0.24076691, -0.10656963, 0.14697677, 0.80735624, 0.18683194, -0.67259216, -0.055239037, -0.5967061, 0.55299073, 0.03418356, 0.5090595, 0.28480536, -0.49888882, -0.87297904, -0.06614408, 0.99944055, 0.81412715, 0.40716594, -0.36394343, -0.33739746, -0.8136965, -0.046265025, 0.45452964, -0.04141159, 0.088112615, -0.4172766, -0.30544177, -0.08896198, 0.27566677, -0.057339653, -0.11345131, 0.20373213, -0.06305934, 0.054166824, -0.07432779, 0.087832116, -0.0669591, -0.20251212, 0.21810754, 0.15648974, 0.22022139, -0.04502745, -0.6723413, 0.22079293, 0.24855897, -1.0515538, -0.020462623, -0.32126144, 0.21761388, 0.20845065, 0.62437844, 0.320426, -0.14200653, -0.18377551, -0.29270753, 0.28922492, 0.018069321, 0.2353616, -0.2527577, -0.49991158, -0.23382792, 0.367332, 0.16444899, 0.28472087, 0.29536533, 0.46435624, 0.45824978, -0.3887685, 0.41376588, 0.015077588, 0.5282044, 0.45758748, -0.3423646, -0.39399993, 0.7598735, 0.519366, -1.0310074, 0.1820696, -0.04453676, -0.2064226, -0.51456296, 0.42674878, 0.8684644, -0.57547593, -0.7519971, 0.5457572, 0.69873565, 0.6031824, -0.6347013, -0.6579183, -0.5419028, 0.52160555, 0.5510167};

    const std::vector<int> genres[N] = {std::vector<int>{2,3,4}, std::vector<int>{1,3,8}, std::vector<int>{4,13}, std::vector<int>{4,7}, std::vector<int>{4}, std::vector<int>{0,5,15}, std::vector<int>{4,13}, std::vector<int>{1,3}, std::vector<int>{0}, std::vector<int>{0,1,15}, std::vector<int>{4,7,13}, std::vector<int>{4,10}, std::vector<int>{2,3}, std::vector<int>{7}, std::vector<int>{0,1,13}, std::vector<int>{7,15}, std::vector<int>{7,13}, std::vector<int>{15}, std::vector<int>{4}, std::vector<int>{0}, std::vector<int>{0,4,7}, std::vector<int>{5,7,15}, std::vector<int>{15}, std::vector<int>{7,14}, std::vector<int>{7,13}, std::vector<int>{7}, std::vector<int>{7}, std::vector<int>{13}, std::vector<int>{1,14}, std::vector<int>{7}, std::vector<int>{7}, std::vector<int>{7,14}, std::vector<int>{1,13}, std::vector<int>{3,4,7}, std::vector<int>{7,13}, std::vector<int>{7}, std::vector<int>{6}, std::vector<int>{4}, std::vector<int>{4,13}, std::vector<int>{7}, std::vector<int>{7,16}, std::vector<int>{0,5,7}, std::vector<int>{7}, std::vector<int>{0,1}, std::vector<int>{4,7}, std::vector<int>{7,13}, std::vector<int>{5,15}, std::vector<int>{2,3,11,13}, std::vector<int>{7,13}, std::vector<int>{5,15}};

    float bias = 0.49290985;

    const int N_GENDER = 2;
    const int N_AGE = 7;
    const int N_OCCUPATION = 21;
    // number of movie feature categories - Genre: 18
    const int N_GENRE = 18;

    const size_t n_items = N - N_GENDER - N_AGE - N_OCCUPATION - N_GENRE;
    
    if (gender >= N_GENDER || gender < 0) {
        return -1;
    }
    if (age >= N_AGE || age < 0) {
        return -1;
    }
    if (occupation >= N_OCCUPATION || occupation < 0) {
        return -1;
    }

    std::vector<int> non_zero_indices;
    non_zero_indices.push_back(gender);
    non_zero_indices.push_back(N_GENDER + age);
    non_zero_indices.push_back(N_GENDER + N_AGE + occupation);

    
    size_t best_id = 0;
    float best_rating = 0.0;

    int m;
    for (m = 0; m < n_items; m++) { 
        // Compute the rating for the m-th item

        std::vector<int> nzi = non_zero_indices;

        // Add the movie.genre indices
        for(auto it = genres[m].begin(); it != genres[m].end(); ++it) {
            nzi.push_back(N_GENDER + N_AGE + N_OCCUPATION + *it);
        }
        //nzi.push_back(N_GENDER + N_AGE + N_OCCUPATION + 1);

        // Add the movie.id
        nzi.push_back(N_GENDER + N_AGE + N_OCCUPATION + N_GENRE + m);

        float rating = fm_predict(order_1, (float*)order_2, RANK, bias, &nzi);
        if(rating > best_rating) {
            best_rating = rating;
            best_id = m;
        }
    }

    return best_id; 

}

