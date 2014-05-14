#include <iostream>
#include <netcdf>
#include <map>
#include <string>
#include <ctime>

using namespace std;
using namespace netCDF;
using namespace netCDF::exceptions;

// Return this in event of a problem.
static const int NC_ERR = 2;

int main(int argc, char **argv) {
	try {
		// http://sac.csic.es/spei/map/maps.html
		NcFile spei_file("/mnt/hgfs/forvm/spei01.nc", NcFile::read);

		cout << "Dimensions: " << endl;
		// cout << spei_file.getDimCount() << endl;
		multimap<string, NcDim> dims = spei_file.getDims();
		multimap<string, NcDim>::iterator itd = dims.begin();
		for (; itd != dims.end(); itd++) {
			// cout << itd->first << ": " << itd->second.getId() << ", " << itd->second.getName() << ", " << itd->second.getSize() << endl;
			cout << itd->second.getName() << " = " << itd->second.getSize() << endl;
		}

		cout << endl;
		cout << "Attributes: " << endl;
		cout << spei_file.getAttCount() << endl;

		cout << endl;
		cout << "Variables: " << endl;
		// cout << spei_file.getVarCount() << endl;
		multimap<string, NcVar> vars = spei_file.getVars();
		multimap<string, NcVar>::iterator itv;
		for(itv = vars.begin(); itv != vars.end(); itv++) {
			// cout << itv->first << ": " << itv->second. getId() << ", " << itv->second.getName() << ", " << itv->second.getType().getName() << ", " << itv->second.getAttCount() << ", " << itv->second.getDimCount() << endl;
			cout << itv->second.getType().getName() << " " << itv->second.getName() << "(";
			vector<NcDim> dims = itv->second.getDims();
			vector<NcDim>::iterator itd;
			for (itd = dims.begin(); itd != dims.end(); itd++) {
				cout << itd->getName() << " = " << itd->getSize() << ", ";
			}
			cout << ");" << endl;

			map<string, NcVarAtt> atts = itv->second.getAtts();
			map<string, NcVarAtt>::iterator ita;
			for (ita = atts.begin(); ita != atts.end(); ita ++) {
				cout << itv->second.getName() << ":";
				cout << ita->second.getName() << " = ";
				if (ita->second.getType() == ncChar) {
					string attValue;
					ita->second.getValues(attValue);
					cout << "\"" << attValue << "\"";
					cout << " [" << "ncChar" << "]";
				} else if (ita->second.getType() == ncFloat) {
					float attValue;
					ita->second.getValues(&attValue);
					cout << attValue;
					cout << " [" << "ncFloat" << "]";
				}
				cout << endl;
			}
		}

		cout << endl;
		cout << "data:" << endl;

		vector<size_t> start;
		start.push_back(0);
		vector<size_t> count;
		count.push_back(10);

		cout << endl;
		cout << "lat:" << endl;
		NcVar lat_var = spei_file.getVar("lat");
		double lat[10];
		lat_var.getVar(start, count, lat);
		for (int i = 0; i < 10; i++)
			cout << lat[i] << ", ";
		cout << endl;

		cout << endl;
		cout << "lon:" << endl;
		NcVar lon_var = spei_file.getVar("lon");
		double lon[10];
		lon_var.getVar(start, count, lon);
		for (int i = 0; i < 10; i++)
			cout << lon[i] << ", ";
		cout << endl;

		cout << endl;
		cout << "times:" << endl;
		NcVar time_var = spei_file.getVar("time");
		double times[10];
		time_var.getVar(start, count, times);
		for (int i = 0; i < 10; i++)
			cout << times[i] << ", ";
		cout << endl;

		cout << endl;
		cout << "spei:" << endl;

		clock_t start_time = clock();

		vector<size_t> spei_start;
		spei_start.push_back(0);
		spei_start.push_back(0);
		spei_start.push_back(0);
		vector<size_t> spei_count;
		spei_count.push_back(1);
		spei_count.push_back(360);
		spei_count.push_back(720);
		NcVar spei_var = spei_file.getVar("spei");
		float spei[360][720];
		spei_var.getVar(spei_start, spei_count, spei);
		for (int i = 0; i < 360; i++) {
			for (int j = 0; j < 720; j++)
				cout << spei[i][j] << "\t";
			cout << endl;
		}

		clock_t end_time = clock();
		cout << "elapsed time: " << (double)(end_time - start_time) / CLOCKS_PER_SEC;

		return 0;
	} catch (NcException &e) {
		e.what();
		return NC_ERR;
	}
}
