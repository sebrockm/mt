#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <unistd.h>



using namespace std;


void usage(char* progname)
{
    cerr << "usage:" << endl;
    cerr << progname << " -n <n> -m <m> [-count <count>] [-type <type>] [-dominter <min> <max>] [-dueinter <min2> <max2>] [-dom [<d1> [<d2> ...]]] [-t <t>] [-ndomt <ndomt>]" << endl;
    cerr << "          <n>: Anzahl Jobs" << endl;
    cerr << "          <m>: Anzahl Maschinen" << endl;
    cerr << "      <count>: Anzahl zu erzeugender Dateien. Default: 1" << endl;
    cerr << "       <type>: cmax oder lmax. Default: cmax" << endl;
    cerr << "  <min> <max>: Intervall, in dem die zufälligen Prozesszeiten der Jobs auf dominierenden Maschinen liegen. Default: 1, 10" << endl;
    cerr << "<min2> <max2>: Intervall, in dem die zufälligen Duedates der Jobs liegen. Default: <min>*<n>, <max>*<n>" << endl;
    cerr << "  <d1>...<di>: Nummern der Maschinen, die dominierend sein sollen. Müssen zwischen 1 und <m> liegen. Default: alle sind dominierend" << endl;
    cerr << "      <ndomt>: Prozesszeit der Jobs auf den nicht dominierenden Maschinen. Default: 0" << endl;
    cerr << "          <t>: Zeitlimit in Stunden für Cplex. Default: 24" << endl;
}

int main(int argc, char** argv)
{
    int n = -1, m = -1;
    int count = 1;
    string type = "cmax";
    int minR = 1, maxR = 10;
    int minD = -1, maxD = -1;
    float t = 24;
    int ndomt = 0;
    vector<int> dom;

    for(int i = 1; i < argc; i++)
    {
        if(argv[i] == string("-n"))
        {
            if(++i >= argc)
            {
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
            n = atoi(argv[i]);
            if(n <= 0)
            {
                cerr << "Jobanzahl muss positiv sein" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(argv[i] == string("-m"))
        {
            if(++i >= argc)
            {
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
            m = atoi(argv[i]);
            if(m <= 0)
            {
                cerr << "Maschinenanzahl muss positiv sein" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(argv[i] == string("-count"))
        {
            if(++i >= argc)
            {
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
            count = atoi(argv[i]);
            if(count <= 0)
            {
                cerr << "Anzahl zu erstellender Dateien muss positiv sein" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(argv[i] == string("-type"))
        {
            if(++i >= argc)
            {
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
            type = argv[i];
            if(type != "cmax" && type != "lmax")
            {
                cerr << "<type> muss cmax oder lmax sein" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(argv[i] == string("-dominter"))
        {
            if(i+2 >= argc)
            {
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
            minR = atoi(argv[++i]);
            maxR = atoi(argv[++i]);

            if(minR > maxR)
                swap(minR, maxR);
        }
        else if(argv[i] == string("dueinter"))
        {
            if(i+2 >= argc)
            {
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
            minD = atoi(argv[++i]);
            maxD = atoi(argv[++i]);

            if(minD > maxD)
                swap(minD, maxD);
        }
        else if(argv[i] == string("-dom"))
        {
            if(m < 0)
            {
                cerr << "-m vor -dom angeben" << endl;
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
            while(++i < argc && argv[i][0] != '-')
            {
                int tmp = atoi(argv[i]);
                if(tmp < 1 || tmp > m)
                {
                    usage(argv[0]);
                    exit(EXIT_FAILURE);
                }
                dom.push_back(tmp);
            }
            i--;
        }
        else if(argv[i] == string("-t"))
        {
            if(++i >= argc)
            {
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
            t = atof(argv[i]);
            if(t <= 0)
            {
                cerr << "Zeitlimit muss positiv sein" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(argv[i] == string("-ndomt"))
        {
            if(++i >= argc)
            {
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
            ndomt = atoi(argv[i]);
            if(ndomt < 0)
            {
                cerr << "Prozesszeit auf nicht dominierten Maschinen darf nicht negativ sein" << endl;
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if(minD < 0 || maxD < 0)
    {
        minD = n*minR;
        maxD = n*maxR;
    }

    if(n < 0 || m < 0 || minR < 0 || maxR < 0)
    {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    stringstream dateistr;
    dateistr << "bench_" << type << "_n" << n << "_m" << m;

    if(dom.size() == 0)//keine dominierenden Maschinen angegeben
    {
        for(int j = 0; j < m; j++)
            dom.push_back(j+1);
    }
    else
    {
        dateistr << "_d";
        for(unsigned j = 0; j < dom.size(); j++)
        {
            dateistr << dom[j];
            if(j < dom.size()-1)
                dateistr << "_";
        }
    }

    string datei = dateistr.str();

    srand(time(0));

    //Hauptschleife
    for(int k = 1; k <= count; k++)
    {
        stringstream dateidat;
        dateidat << datei << ".dat" << k;
        ofstream file(dateidat.str().c_str());
        if(!file)
        {
            cerr << "kann " << dateidat.str() << " nicht öffnen" << endl;
            exit(EXIT_FAILURE);
        }

        file << m << " " << n << endl;
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < m; j++)
            {
                if(find(dom.begin(), dom.end(), j+1) == dom.end())//j ist nicht dominierend
                {
                    file << ndomt << " ";
                }
                else
                {
                    file << minR + rand()%(maxR-minR+1) << " ";
                }
            }
            file << endl;
        }
        file << endl;

        if(type == "lmax")
        {
            for(int i = 1; i <= n; i++)
            {
                file << i << " " << minD + rand()%(maxD-minD+1) << endl;
            }
        }

        file.close();

        string zpl;
        if(type == "cmax")
        {
            zpl = "synmvCmax";
        }
        else
        {
            zpl = "synmvLmax";
        }

        stringstream zplss;
        zplss << zpl << ".zpl";
        ifstream zplfile(zplss.str().c_str());
        if(!zplfile)
        {
            cerr << "kann " << zplss.str() << "nicht öffnen" << endl;
            exit(EXIT_FAILURE);
        }
        vector<string> lines;
        string line;
        while(getline(zplfile, line))
            lines.push_back(line);
        zplfile.close();

        lines[0] = "param file := \"" + dateidat.str() + "\";";
        ofstream nzplfile(zplss.str().c_str(), ofstream::trunc);
        if(!nzplfile)
        {
            cerr << "kann " << zplss.str() << " nicht öffnen" << endl;
            exit(EXIT_FAILURE);
        }

        for(unsigned i = 0; i < lines.size(); i++)
            nzplfile << lines[i] << endl;
        nzplfile.close();

        system(("zimpl " + zplss.str()).c_str());
        stringstream cplexss;
        cplexss << "cplex -c ";
        cplexss << "\"set timelimit " << (int)(t*60*60) << "\" ";
        cplexss << "\"read " << zpl << ".lp\" ";
        cplexss << "\"optimize\" \"quit\" ";
        cplexss << "> " << datei << ".log" << k;
        
        system(cplexss.str().c_str());

        stringstream logss;
        logss << datei << ".log" << k;
	system(("cat "+logss.str()).c_str());
        ifstream log(logss.str().c_str());
        if(!log)
        {
            cerr << "kann " << logss.str() << " nicht öffnen" << endl;
            exit(EXIT_FAILURE);
        }

        while(getline(log, line))
        {
            size_t pos = line.find("integer feasible:");
	    size_t pos2 = line.find("Integer optimal");
            if(pos != string::npos || pos2 != string::npos)
                break;
        }
        vector<string> datlines;
	if(line.find("nteger") != string::npos)
		datlines.push_back('#'+line);
        getline(log, line);
	if(line.find("bound") != string::npos)
		datlines.push_back('#'+line);

        ifstream idat(dateidat.str().c_str());
        if(!idat)
        {
            cerr << "kann " << dateidat.str() << " nicht öffnen" << endl;
            exit(EXIT_FAILURE);
        }
        while(getline(idat, line))
            datlines.push_back(line);

        ofstream odat(dateidat.str().c_str(), ofstream::trunc);
        if(!odat)
        {
            cerr << "kann " << dateidat.str() << " nicht öffnen" << endl;
            exit(EXIT_FAILURE);
        }
        for(unsigned i = 0; i < datlines.size(); i++)
            odat << datlines[i] << endl;
    }
}
