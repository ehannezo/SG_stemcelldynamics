void writedata_cumul()
{
    string datname;
    
    
    
    int filenichtda=0;
    
    datname="cumul";
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
    
    
    
    for(int p=0;p<100;p++)
    {
        for(int i=0;i<1000;i++)
        {
            data  << cumul[p][i] << " "   ;
        }
        data << "\n";
        
    }
    
    
    data.close();
    
}
