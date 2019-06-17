void writedata_mean()
{
	string datname;
	


	int filenichtda=0;
	
	datname="mean";
	datname+= ".txt";
	ifstream testles(datname.c_str());
	if (!testles.good()) {
		filenichtda=1;
	}
	

	ofstream data(datname.c_str(),ios::out|ios::app);
	while (!data.good())
	{
		data.clear();
		data.open(datname.c_str(),ios::out|ios::app);
		cerr << "writing to measurements.txt,ios::out|ios::app faild" << endl;
		if (data.good()) cerr << " writing works" << endl;
	}



            for(int i=0;i<T;i++)
            {
                data << i << " " << mean_p[i] << " " << mean_d[i]  ;
                data << "\n";

            }


        

  data.close();
 
}
