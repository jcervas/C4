// g++ vor.cxx -I ~/anaconda3/include/ -std=c++11 -o vor

#include <iostream>
#include <fstream>

#include <unordered_set>
#include <algorithm>
#include <random>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>
#include <boost/geometry/algorithms/union.hpp>

#include <boost/polygon/voronoi.hpp>
#include <boost/geometry/geometries/adapted/boost_polygon.hpp>


#include "ehrenburg.hpp"


#define DEBUG_ME cerr << __FILE__ << "::" << __LINE__ << "\t" << __FUNCTION__ << endl


using std::cout;
using std::cerr;
using std::endl;

namespace bgeo  = boost::geometry;
namespace bpoly = boost::polygon;

typedef bpoly::point_data<double> bp_pt;
typedef bpoly::segment_data<double> bp_seg;

namespace bgmod = boost::geometry::model;
typedef bgmod::d2::point_xy<double> bg_pt;
typedef bgmod::multi_point<bp_pt>   bg_mpt; // note BP point (not BG), for Voronoi.
typedef bgmod::polygon<bp_pt>       bg_poly;
typedef bgmod::linestring<bp_pt>    bg_lstr;
typedef bgmod::ring<bp_pt>          bg_ring;
typedef bgmod::multi_polygon<bg_poly> bg_mpoly;


int main()
{

    std::unordered_set<int> a;
    for (int i = 0; i < 10; i++) a.insert(i);

    for (auto ax : a) cout << ax << " ";
    cout << endl;

    for (auto ax : a) cout << ax << " ";
    cout << endl;

    auto engine = std::default_random_engine();

    std::vector<std::unordered_set<int>::iterator> aiv(a.size());
    std::iota(aiv.begin(), aiv.end(), a.begin());

    for (int i = 0; i < 10; i++) {
      std::shuffle(aiv.begin(), aiv.end(), engine);
      for (auto ax : aiv) cout << *ax << " ";
      cout << endl;
    }


    bg_pt pt1(2, 1.3);

    bg_mpoly mpoly1, mpoly2, mpoly_o;
    boost::geometry::read_wkt("MULTIPOLYGON(())", mpoly1); 
    boost::geometry::read_wkt("MULTIPOLYGON(((1670256 -300731,1670302 -300663,1671350 -301199,1671459 -301414,1671543 -301575,1670256 -300731)), ())", mpoly2);

    bgeo::union_(mpoly1, mpoly2, mpoly1);
    // cout << bgeo::area(mpoly1) << endl;
    // cout << bgeo::area(mpoly2) << endl;
    // cout << bgeo::area(mpoly_o) << endl;

    bg_mpt mpt;
    bg_mpt mpt_diff;
    std::vector<bg_pt> pts;
    for (double i = 0; i < 100.; i += 1.23) {
      pts.push_back(bg_pt(i, i/2));
    }

    for (auto p : pts) {
      bgeo::append(mpt, p);
      //cout << "N=" << bgeo::num_points(mpt) << endl;
    }

    for (auto p : pts) {
      bgeo::clear(mpt_diff);
      bgeo::difference(mpt, p, mpt_diff);
      mpt = mpt_diff;
      // cout << "N=" << bgeo::num_points(mpt) << " or " << bgeo::num_points(mpt_diff) << endl;
    }

    bg_poly poly;
    // boost::geometry::read_wkt("polygon((0 500, -150 200, -250 100, -250 0, 250 0, 250 100, 150 200, 0 500))", poly);
    boost::geometry::read_wkt("POLYGON((1994498 -260094,"
        "1995636 -259454,"
        "1995250 -258443,"
        "1995952 -257942,"
        "1996209 -256968,"
        "1996467 -256837,"
        "1996798 -256909,"
        "1997017 -256648,"
        "1998331 -256321,"
        "1998823 -255837,"
        "1999133 -255767,"
        "1999289 -255393,"
        "1999733 -255361,"
        "2000049 -254987,"
        "2000448 -254942,"
        "2002147 -255455,"
        "2003213 -255543,"
        "2004160 -255340,"
        "2005290 -255625,"
        "2005183 -254965,"
        "2005475 -254489,"
        "2005509 -253564,"
        "2006114 -253288,"
        "2007067 -252409,"
        "2007135 -251921,"
        "2007009 -251607,"
        "2007712 -251883,"
        "2007896 -251740,"
        "2008072 -250818,"
        "2007738 -250641,"
        "2008112 -249824,"
        "2008812 -249346,"
        "2010396 -248694,"
        "2010413 -248155,"
        "2011087 -247836,"
        "2010991 -247563,"
        "2012063 -247320,"
        "2011934 -247036,"
        "2016690 -242319,"
        "1999565 -234629,"
        "2000248 -233695,"
        "2000502 -230467,"
        "1999946 -230384,"
        "1999205 -230895,"
        "1999025 -230794,"
        "1998511 -229814,"
        "1997988 -229468,"
        "1997815 -228938,"
        "1997376 -228539,"
        "1998385 -227106,"
        "1999616 -227299,"
        "1999514 -226930,"
        "1999664 -226854,"
        "1999665 -226580,"
        "2000303 -226960,"
        "2000492 -226155,"
        "1999909 -226028,"
        "2000152 -225560,"
        "2000024 -225427,"
        "2000414 -225231,"
        "2000545 -225565,"
        "2001031 -225402,"
        "2001133 -225726,"
        "2001603 -225572,"
        "2001416 -225905,"
        "2001287 -226919,"
        "2001461 -227530,"
        "2002326 -226425,"
        "2003305 -226633,"
        "2003189 -227266,"
        "2003311 -227382,"
        "2003494 -227270,"
        "2003343 -227732,"
        "2003889 -227703,"
        "2003924 -227348,"
        "2004057 -227332,"
        "2004535 -228659,"
        "2005297 -227973,"
        "2005683 -226805,"
        "2005846 -226696,"
        "2006051 -227261,"
        "2006074 -228251,"
        "2006617 -229362,"
        "2006649 -230151,"
        "2007672 -229815,"
        "2007613 -230119,"
        "2008392 -230277,"
        "2008717 -229961,"
        "2008879 -230094,"
        "2010343 -227033,"
        "2010331 -225502,"
        "2010641 -225743,"
        "2011654 -225989,"
        "2013045 -225767,"
        "2014117 -225970,"
        "2014775 -226512,"
        "2016122 -226328,"
        "2016638 -226435,"
        "2016935 -226590,"
        "2016799 -228249,"
        "2017924 -228147,"
        "2018343 -228321,"
        "2018515 -228052,"
        "2018984 -228005,"
        "2018529 -223455,"
        "2018353 -219614,"
        "2017927 -219613,"
        "2016522 -219118,"
        "2015771 -218285,"
        "2014751 -217717,"
        "2014554 -217213,"
        "2014070 -216805,"
        "2014078 -214520,"
        "2012950 -214081,"
        "2014024 -212001,"
        "2014166 -210151,"
        "2019173 -210169,"
        "2019455 -210954,"
        "2021020 -210997,"
        "2021060 -211641,"
        "2026499 -212306,"
        "2025569 -215516,"
        "2030110 -217571,"
        "2027713 -226580,"
        "2024711 -230515,"
        "2025893 -231404,"
        "2027194 -231457,"
        "2028000 -231932,"
        "2028402 -232528,"
        "2027914 -233217,"
        "2028272 -233484,"
        "2028514 -234044,"
        "2028760 -234116,"
        "2029446 -233653,"
        "2029640 -233674,"
        "2029906 -234544,"
        "2030266 -234570,"
        "2030375 -234978,"
        "2030077 -235129,"
        "2029796 -235671,"
        "2029456 -235896,"
        "2029479 -236113,"
        "2029053 -236311,"
        "2028988 -236580,"
        "2028552 -236954,"
        "2027063 -237832,"
        "2026983 -237703,"
        "2026751 -237744,"
        "2026117 -238019,"
        "2025989 -238359,"
        "2025194 -238224,"
        "2024891 -238621,"
        "2024563 -238555,"
        "2023696 -238815,"
        "2023225 -238307,"
        "2022066 -238495,"
        "2021203 -238856,"
        "2022455 -240118,"
        "2022770 -241152,"
        "2024607 -242225,"
        "2024909 -242293,"
        "2024699 -241794,"
        "2024813 -241431,"
        "2025796 -241034,"
        "2026050 -241565,"
        "2027765 -243273,"
        "2028768 -243126,"
        "2030221 -245855,"
        "2030393 -246363,"
        "2026944 -247845,"
        "2027366 -249434,"
        "2026540 -249777,"
        "2026984 -252618,"
        "2027380 -252448,"
        "2027781 -253695,"
        "2028128 -253542,"
        "2028075 -253411,"
        "2029230 -252905,"
        "2029529 -253615,"
        "2029739 -253538,"
        "2029783 -253757,"
        "2029638 -253887,"
        "2030180 -255031,"
        "2032993 -253507,"
        "2033091 -253751,"
        "2033746 -253528,"
        "2033808 -253690,"
        "2033221 -254079,"
        "2033287 -254805,"
        "2032370 -256133,"
        "2032211 -256784,"
        "2032317 -257226,"
        "2032708 -257813,"
        "2033372 -258237,"
        "2033941 -257641,"
        "2033887 -257505,"
        "2035190 -256710,"
        "2035785 -256673,"
        "2036218 -256847,"
        "2036908 -257974,"
        "2036328 -258626,"
        "2037023 -259185,"
        "2037124 -259450,"
        "2037474 -259450,"
        "2038029 -259876,"
        "2038251 -259796,"
        "2038273 -260804,"
        "2038739 -260028,"
        "2038566 -259460,"
        "2039530 -259086,"
        "2038540 -258192,"
        "2039943 -256628,"
        "2039591 -256280,"
        "2039836 -256146,"
        "2040267 -256129,"
        "2042036 -257752,"
        "2042292 -257584,"
        "2042728 -257703,"
        "2043510 -258429,"
        "2044232 -258621,"
        "2044316 -258280,"
        "2046000 -259860,"
        "2046152 -259688,"
        "2046942 -260411,"
        "2048109 -259096,"
        "2049053 -259993,"
        "2049274 -259766,"
        "2048973 -259337,"
        "2049149 -259136,"
        "2049458 -259432,"
        "2050103 -258717,"
        "2049616 -258257,"
        "2050150 -257682,"
        "2049841 -257192,"
        "2050056 -256926,"
        "2049858 -256743,"
        "2050307 -256281,"
        "2051024 -256975,"
        "2051230 -256757,"
        "2051389 -256898,"
        "2051536 -256697,"
        "2050696 -255911,"
        "2050951 -255487,"
        "2051189 -255811,"
        "2051554 -255345,"
        "2051363 -254893,"
        "2050857 -254443,"
        "2056660 -247840,"
        "2056248 -246917,"
        "2056065 -247215,"
        "2055818 -247073,"
        "2054994 -247360,"
        "2057887 -242472,"
        "2056577 -241186,"
        "2060173 -237269,"
        "2059507 -236677,"
        "2057776 -238556,"
        "2054468 -235381,"
        "2053239 -236611,"
        "2052860 -236171,"
        "2051820 -237264,"
        "2051595 -236620,"
        "2050981 -236412,"
        "2050896 -235759,"
        "2051748 -234970,"
        "2052952 -234265,"
        "2053753 -234295,"
        "2054381 -234105,"
        "2055939 -232519,"
        "2056367 -232308,"
        "2056915 -230834,"
        "2057197 -230951,"
        "2057590 -229852,"
        "2056774 -229514,"
        "2057293 -228120,"
        "2056124 -227915,"
        "2048546 -224901,"
        "2048289 -224180,"
        "2048139 -224251,"
        "2048084 -224120,"
        "2048513 -224014,"
        "2048457 -223634,"
        "2047785 -222385,"
        "2047442 -220865,"
        "2046937 -220268,"
        "2046718 -219568,"
        "2046990 -219683,"
        "2047200 -219535,"
        "2047098 -219163,"
        "2047392 -219062,"
        "2047514 -218796,"
        "2049610 -219587,"
        "2049737 -219262,"
        "2053131 -220563,"
        "2055479 -214400,"
        "2055655 -214280,"
        "2059496 -215829,"
        "2059195 -216633,"
        "2060535 -217207,"
        "2060447 -217462,"
        "2061287 -217792,"
        "2060946 -219266,"
        "2061985 -219526,"
        "2061858 -220055,"
        "2062492 -220243,"
        "2062759 -220222,"
        "2062667 -219896,"
        "2062820 -219939,"
        "2062927 -219639,"
        "2063055 -219697,"
        "2063291 -218857,"
        "2062407 -218501,"
        "2062947 -217185,"
        "2064634 -217859,"
        "2063906 -219785,"
        "2065341 -220345,"
        "2065838 -218875,"
        "2066139 -218925,"
        "2066336 -218539,"
        "2067602 -219064,"
        "2067791 -218491,"
        "2067317 -218004,"
        "2067148 -218065,"
        "2068355 -214483,"
        "2069063 -215696,"
        "2068710 -216689,"
        "2071403 -217751,"
        "2071580 -217893,"
        "2071782 -218666,"
        "2072219 -218981,"
        "2072391 -218532,"
        "2072133 -218467,"
        "2072348 -218124,"
        "2073835 -218716,"
        "2073675 -219123,"
        "2074229 -219496,"
        "2073578 -219593,"
        "2073246 -218771,"
        "2072305 -219504,"
        "2072419 -219679,"
        "2073494 -220097,"
        "2073390 -220464,"
        "2073657 -220592,"
        "2073798 -220309,"
        "2074035 -220304,"
        "2074403 -220444,"
        "2074692 -219630,"
        "2075570 -219951,"
        "2074803 -221917,"
        "2072388 -220971,"
        "2071765 -222684,"
        "2071373 -221970,"
        "2071096 -222558,"
        "2069700 -222011,"
        "2069780 -221790,"
        "2069126 -221171,"
        "2068470 -222924,"
        "2067873 -222402,"
        "2067110 -221014,"
        "2066839 -221747,"
        "2065743 -221282,"
        "2065677 -221634,"
        "2065002 -221399,"
        "2064609 -222438,"
        "2064666 -222858,"
        "2067646 -223891,"
        "2067159 -224605,"
        "2066543 -225005,"
        "2066093 -225937,"
        "2067253 -226410,"
        "2067757 -224885,"
        "2069852 -225709,"
        "2069890 -225486,"
        "2069996 -225529,"
        "2070281 -224780,"
        "2070200 -223774,"
        "2069572 -223527,"
        "2069836 -222802,"
        "2070862 -223220,"
        "2070403 -224472,"
        "2070635 -224565,"
        "2070466 -225008,"
        "2070765 -225519,"
        "2071038 -225566,"
        "2071311 -224818,"
        "2072115 -225139,"
        "2072364 -224216,"
        "2072624 -224365,"
        "2073089 -224845,"
        "2072947 -225450,"
        "2073579 -225838,"
        "2073709 -225784,"
        "2073359 -226773,"
        "2073019 -226633,"
        "2072882 -226994,"
        "2074233 -227555,"
        "2073754 -228822,"
        "2072161 -228298,"
        "2071887 -228564,"
        "2070848 -228970,"
        "2070493 -228966,"
        "2069445 -231935,"
        "2068506 -232429,"
        "2068062 -232166,"
        "2068593 -231223,"
        "2069188 -230852,"
        "2068974 -230377,"
        "2067113 -230675,"
        "2065949 -230248,"
        "2066173 -229613,"
        "2065345 -230094,"
        "2065097 -229537,"
        "2064453 -228778,"
        "2064223 -229371,"
        "2063502 -229061,"
        "2063492 -229873,"
        "2063264 -230173,"
        "2062862 -230246,"
        "2062857 -231240,"
        "2062529 -231712,"
        "2061740 -231721,"
        "2061881 -229766,"
        "2062789 -227321,"
        "2062235 -227113,"
        "2062753 -226582,"
        "2062914 -226143,"
        "2062783 -226007,"
        "2063260 -224784,"
        "2062592 -224521,"
        "2060313 -226402,"
        "2059946 -226262,"
        "2059639 -227127,"
        "2059660 -227611,"
        "2059401 -228077,"
        "2059556 -228316,"
        "2059218 -228687,"
        "2059106 -228642,"
        "2058105 -231401,"
        "2058351 -231499,"
        "2058199 -231907,"
        "2059120 -231793,"
        "2059881 -232004,"
        "2059883 -232190,"
        "2059616 -232284,"
        "2056987 -232084,"
        "2057081 -232445,"
        "2057394 -232558,"
        "2058285 -233522,"
        "2057552 -233796,"
        "2056627 -234641,"
        "2056452 -234548,"
        "2056104 -234943,"
        "2055235 -235337,"
        "2055451 -235689,"
        "2055202 -235872,"
        "2055672 -235790,"
        "2057409 -236236,"
        "2060720 -236033,"
        "2061609 -236436,"
        "2062814 -236604,"
        "2062185 -237237,"
        "2061890 -237918,"
        "2061505 -238338,"
        "2064973 -241566,"
        "2065606 -240945,"
        "2069770 -244678,"
        "2068498 -246143,"
        "2069418 -246861,"
        "2069649 -246694,"
        "2070476 -247159,"
        "2070122 -247561,"
        "2070522 -247846,"
        "2069767 -248660,"
        "2070736 -249810,"
        "2070684 -250440,"
        "2071556 -250437,"
        "2070644 -251456,"
        "2070775 -251593,"
        "2069960 -252343,"
        "2070684 -252770,"
        "2070994 -252418,"
        "2071317 -252561,"
        "2070182 -253830,"
        "2070484 -254059,"
        "2071132 -254168,"
        "2071618 -254743,"
        "2071717 -254659,"
        "2071863 -254803,"
        "2071722 -255235,"
        "2071881 -255365,"
        "2072329 -255093,"
        "2072806 -255047,"
        "2073038 -255359,"
        "2073246 -255376,"
        "2073725 -254585,"
        "2074053 -255139,"
        "2073281 -255586,"
        "2071775 -257015,"
        "2070265 -257531,"
        "2070179 -257156,"
        "2069804 -256915,"
        "2070126 -256765,"
        "2069598 -256379,"
        "2069694 -256791,"
        "2068854 -257786,"
        "2068845 -258148,"
        "2068299 -258304,"
        "2068051 -258162,"
        "2067533 -258777,"
        "2067240 -258484,"
        "2066480 -259426,"
        "2065959 -258954,"
        "2066318 -258710,"
        "2066217 -258095,"
        "2066611 -257793,"
        "2066610 -257994,"
        "2066816 -256858,"
        "2066683 -256606,"
        "2066391 -256699,"
        "2066028 -256379,"
        "2065987 -255935,"
        "2066086 -255746,"
        "2066708 -255932,"
        "2067261 -255126,"
        "2067144 -255057,"
        "2067341 -254885,"
        "2067036 -254450,"
        "2067142 -254269,"
        "2066910 -254207,"
        "2067079 -253901,"
        "2066372 -253119,"
        "2065707 -253434,"
        "2065611 -253741,"
        "2065484 -253692,"
        "2065309 -253908,"
        "2065067 -253752,"
        "2065189 -253565,"
        "2064954 -253375,"
        "2064895 -253085,"
        "2064527 -253059,"
        "2064727 -252714,"
        "2064597 -252477,"
        "2064494 -252857,"
        "2064323 -252851,"
        "2064358 -252535,"
        "2064087 -252444,"
        "2064113 -251942,"
        "2063156 -253566,"
        "2063598 -254171,"
        "2063205 -254376,"
        "2063043 -254244,"
        "2062838 -255782,"
        "2062950 -256375,"
        "2062801 -256625,"
        "2063723 -257021,"
        "2063739 -257267,"
        "2064076 -257605,"
        "2064186 -258298,"
        "2064482 -258445,"
        "2064757 -258250,"
        "2064978 -258334,"
        "2065355 -259116,"
        "2065351 -259635,"
        "2064723 -259736,"
        "2064842 -260147,"
        "2064399 -260190,"
        "2064405 -260304,"
        "2063680 -259927,"
        "2063872 -260050,"
        "2063929 -260885,"
        "2063523 -260684,"
        "2063134 -260935,"
        "2062619 -260710,"
        "2062121 -261275,"
        "2063857 -262828,"
        "2063780 -262918,"
        "2064094 -263268,"
        "2064210 -263364,"
        "2064383 -263194,"
        "2065687 -264902,"
        "2064817 -266150,"
        "2064077 -267038,"
        "2062800 -266579,"
        "2059491 -265894,"
        "2056326 -265762,"
        "2054648 -265930,"
        "2052899 -266250,"
        "2049615 -267230,"
        "2046731 -268654,"
        "2045439 -269503,"
        "2043443 -271177,"
        "2041881 -272844,"
        "2039962 -275671,"
        "2038882 -277908,"
        "2038098 -280276,"
        "2037655 -282737,"
        "2037509 -284614,"
        "2034413 -285597,"
        "2033797 -284909,"
        "2033385 -283892,"
        "2034009 -283600,"
        "2033380 -282029,"
        "2033537 -281959,"
        "2033476 -281442,"
        "2033054 -280837,"
        "2033885 -280366,"
        "2033577 -279706,"
        "2034056 -279525,"
        "2033924 -279201,"
        "2035487 -278360,"
        "2033940 -274302,"
        "2034379 -274098,"
        "2033975 -272972,"
        "2033463 -273189,"
        "2032707 -271237,"
        "2038136 -268723,"
        "2038210 -268893,"
        "2038542 -268752,"
        "2038623 -269082,"
        "2037743 -269533,"
        "2038554 -271163,"
        "2039874 -270276,"
        "2039104 -268835,"
        "2039043 -268326,"
        "2041966 -266911,"
        "2041360 -266355,"
        "2041590 -266130,"
        "2042562 -265919,"
        "2044635 -267429,"
        "2043793 -265296,"
        "2043405 -265148,"
        "2043286 -264188,"
        "2042441 -263161,"
        "2040828 -264590,"
        "2038229 -263821,"
        "2030419 -264044,"
        "2030286 -259030,"
        "2030625 -259035,"
        "2030631 -258184,"
        "2029828 -256100,"
        "2028754 -256558,"
        "2028425 -256194,"
        "2028256 -256261,"
        "2027847 -255296,"
        "2027493 -254995,"
        "2027590 -254694,"
        "2027353 -254089,"
        "2025527 -255180,"
        "2024651 -255354,"
        "2021081 -257735,"
        "2020911 -258606,"
        "2021045 -259094,"
        "2020124 -259457,"
        "2020443 -260078,"
        "2020122 -260901,"
        "2020365 -261393,"
        "2019340 -261992,"
        "2019074 -261287,"
        "2018090 -261689,"
        "2017814 -262392,"
        "2017531 -262400,"
        "2017758 -262985,"
        "2017387 -263358,"
        "2017440 -263487,"
        "2017883 -263303,"
        "2018117 -263914,"
        "2020802 -261994,"
        "2021773 -262715,"
        "2022432 -262557,"
        "2022807 -263171,"
        "2023223 -263270,"
        "2023800 -262952,"
        "2024561 -263880,"
        "2024360 -264064,"
        "2024506 -264531,"
        "2024968 -264833,"
        "2025471 -264844,"
        "2026212 -266090,"
        "2026475 -266361,"
        "2026655 -266217,"
        "2027035 -266873,"
        "2025954 -268560,"
        "2027287 -271685,"
        "2026857 -271866,"
        "2029441 -277882,"
        "2024549 -279817,"
        "2024302 -279443,"
        "2024473 -279064,"
        "2024295 -278824,"
        "2024385 -278469,"
        "2022745 -278896,"
        "2021215 -278131,"
        "2018982 -278021,"
        "2018276 -277571,"
        "2017621 -277759,"
        "2017331 -277567,"
        "2017089 -277810,"
        "2015533 -277872,"
        "2015349 -278283,"
        "2015496 -278524,"
        "2014952 -278576,"
        "2014727 -278426,"
        "2014359 -278627,"
        "2014449 -278937,"
        "2014000 -279051,"
        "2014034 -279470,"
        "2013861 -279872,"
        "2013608 -279970,"
        "2012918 -279834,"
        "2012945 -280100,"
        "2013597 -280559,"
        "2013994 -281139,"
        "2013616 -281310,"
        "2013691 -282016,"
        "2013479 -282646,"
        "2012754 -282573,"
        "2012627 -282867,"
        "2011530 -282483,"
        "2011244 -282619,"
        "2010963 -282547,"
        "2010579 -281864,"
        "2009922 -281873,"
        "2009744 -281460,"
        "2008878 -281457,"
        "2008398 -280838,"
        "2008284 -280830,"
        "2008265 -281246,"
        "2007985 -281313,"
        "2006945 -280182,"
        "2006596 -280076,"
        "2006723 -279830,"
        "2006304 -279046,"
        "2006045 -278812,"
        "2005520 -278813,"
        "2005068 -278090,"
        "2004319 -277786,"
        "2004151 -277878,"
        "2003985 -277682,"
        "2003479 -277706,"
        "2003397 -277930,"
        "2003167 -277985,"
        "2002888 -277685,"
        "2002708 -276906,"
        "2002876 -276602,"
        "2002848 -275714,"
        "2006561 -274475,"
        "2002610 -268338,"
        "2001276 -269525,"
        "1997315 -263033,"
        "1997472 -262955,"
        "1997976 -263513,"
        "1998136 -263319,"
        "1998107 -262920,"
        "1998358 -262860,"
        "1998484 -262534,"
        "1999005 -262819,"
        "1999147 -262500,"
        "1999381 -262473,"
        "1994498 -260094))", poly);

    bg_poly hull;
    bgeo::convex_hull(poly, hull);

    // cout << "polygon: " << bgeo::dsv(poly) << ", area: " << bgeo::area(poly) << endl;
    // cout << "pre-2nd hull :    " << bgeo::dsv(hull) << ", area: " << bgeo::area(hull) << "  (2, 1.3) within? " << bgeo::within(pt1, poly) << endl;


    bgeo::clear(hull);
    bgeo::convex_hull(poly, hull);
    // cout << "post-2nd hull:    " << bgeo::dsv(hull) << ", area: " << bgeo::area(hull) << "  (2, 1.3) within? " << bgeo::within(pt1, poly) << endl;


    
    bg_mpt mult;
    for (auto p : bgeo::exterior_ring(poly)) {
      // cout << p.x() << ", " << p.y() << "     within poly? " << (bgeo::within(p, poly) ? "TRUE" : "FALSE") 
      //                                << "     intersects poly? " << (bgeo::intersects(p, poly) ? "TRUE" : "FALSE") << endl;
      bgeo::append(mult, p);
    }

    bg_pt ctr;
    bgeo::centroid(hull, ctr);
    // cout << "The centroid (" << ctr.x() << ", " << ctr.y() << ") must be within a convex shape!  " << bgeo::within(ctr, hull) << endl;

    // cout << "multipoint: " << bgeo::dsv(mult) << endl;

    bg_mpt diff;
    bgeo::difference(mult, pt1, diff);
    // cout << "multipoint post: " << bgeo::dsv(diff) << endl;

    bg_mpt diff_poly;
    // bgeo::difference(poly, pt1, diff_poly);
    // cout << "multipoly post: " << bgeo::dsv(diff_poly) << endl;
    

    bg_lstr line;
    std::vector<bp_seg> sdata2;
    for (auto ipt  = boost::begin(bgeo::exterior_ring(poly));
              ipt != boost::end(bgeo::exterior_ring(poly)); ++ipt) {
      line.push_back(*ipt);
      if (ipt != boost::end(bgeo::exterior_ring(poly))) sdata2.push_back(bp_seg(*ipt, *(ipt+1)));
    }

    bpoly::voronoi_diagram<double> vd;

    cout << "voronoi ...." << endl;
    construct_voronoi(sdata2.begin(), sdata2.end(), &vd);
    cout << "done!!" << endl;

    double d2;
    bp_pt  vd_ctr;
    double max_dist = boost::numeric::bounds<double>::lowest();
    // cout << "vertices: " << vd.num_vertices() << "   edges: " << vd.num_edges() << "  cells: " << vd.num_cells() << endl;

    cout << ">>> ITERATING" << endl;
    for (auto it : vd.vertices()) {

      bp_pt pt(it.x(), it.y());
      
      d2 = bpoly::euclidean_distance(sdata2[it.incident_edge()->cell()->source_index()], pt);
      // d2 = bgeo::comparable_distance(pt, line);

      if (max_dist < d2 && bgeo::within(pt, poly)) {
        max_dist = d2;
        bgeo::set<0>(vd_ctr, it.x());
        bgeo::set<1>(vd_ctr, it.y());
      }
    } 
    cout << "DONE" << endl;

    /*
    cout << "ehrenburg is at (" << vd_ctr.x() << ", " << vd_ctr.y() << ")  with radius " << sqrt(max_dist) << " =? " << bgeo::distance(vd_ctr, line) << endl;

    double tol = 10;
    cout << "now trying numerical search..." << endl;
    // auto pe = ehrenburg::pole_inacc<double, bp_pt>(poly, tol, true);
    auto pe = ehrenburg::pole_inacc<double, bp_pt>(vd_ctr, poly, tol, true);
    cout << "compare result to voronoi: " << pe.first.x() << ", " << pe.first.y() << "  d=" << pe.second << endl;


    cout << ">>> ITERATING -- close only" << endl;
    double tol2 = tol * tol;
    for (auto it : vd.vertices()) {

      bp_pt pt(it.x(), it.y());
      if ( bgeo::comparable_distance(pe.first, pt) > tol2 ||
          !bgeo::within(pt, poly)) continue;

      d2 = bgeo::comparable_distance(pt, line);
      if (max_dist < d2) {
        max_dist = d2;
        bgeo::set<0>(vd_ctr, it.x());
        bgeo::set<1>(vd_ctr, it.y());
      }
    } 
    cout << "DONE : " << sqrt(max_dist) << endl;

    */

    const int points_per_circle = 100;
    boost::geometry::strategy::buffer::distance_symmetric<double> distance_strategy(max_dist);
    boost::geometry::strategy::buffer::join_round join_strategy(points_per_circle);
    boost::geometry::strategy::buffer::end_round end_strategy(points_per_circle);
    boost::geometry::strategy::buffer::point_circle circle_strategy(points_per_circle);
    boost::geometry::strategy::buffer::side_straight side_strategy;

    // Create the buffer of a multi point
    boost::geometry::model::multi_polygon<bg_poly> vd_buff;
    boost::geometry::buffer(vd_ctr, vd_buff, distance_strategy, side_strategy,
                                             join_strategy, end_strategy, circle_strategy);

    // Declare a stream and an SVG mapper
    std::ofstream svg("my_map.svg");
    boost::geometry::svg_mapper<bp_pt> mapper(svg, 400, 400, "width=\"120%\" height=\"120%\"");

    // Add geometries such that all these geometries fit on the map
    mapper.add(poly);
    mapper.add(vd_ctr);
    mapper.add(vd_buff);

    // Draw the geometries on the SVG map, using a specific SVG style
    mapper.map(poly,    "fill-opacity:0.3;fill:rgb(128,122,117);");
    mapper.map(vd_buff, "fill-opacity:0.3;fill:rgb(139,0,33);stroke:rgb(139,0,33);stroke-width:0,2", 5);
    mapper.map(vd_ctr,  "fill-opacity:1.0;fill:rgb(139,0,33);stroke:rgb(139,0,33);stroke-width:0,2", 5);

    return 0;

}
