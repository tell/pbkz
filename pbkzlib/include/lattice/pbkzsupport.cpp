#ifndef _inc_pbkzsupport_cpp
#define _inc_pbkzsupport_cpp

#include "pbkzsharemem.cpp"




// Estimate (Hermite factor)^(1/n) 
double Estimate_delta(quad_float* c,int bs) {
    //double r = exp(1.0 * to_double(log(c[bs-1]/c[0]))/(bs-1.0));
    //cout << "delta1=" << exp(-log(r)*(bs-1.0)/(4*bs)) << endl;
    
    //Estimating delta by least square
    double r,xs,ys,xxs;
    int i;
    xs = bs*(bs-1.0)/2.0;
    xxs = bs*(bs-1.0)*(2.0*bs-1.0)/6.0;
    ys = 0;
    for (i=0;i<bs;i++) ys += 0.5*log(to_double(c[i]));
    r = 0;
    for (i=0;i<bs;i++) r += 0.5*i*log(to_double(c[i]));
    r = (1.0*bs*r - xs*ys) / (1.0*bs*xxs-xs*xs);
    r = exp(2.0*r);
    double ret = exp(-log(r)*(bs-1.0)/(4.0*bs)); 
    if ((ret<=0.9) || (ret>=1.1) ) {
        if (bs>10) {
            cout << "delta-error: " << endl;
            for (i=0;i<bs;i++) {
                cout << "c[" << i << "]=" << c[i] << endl;
            }
        }
    }
    return  ret;
    
}


double InitRadius(quad_float* c,int jj,int bs,double radius,char mode) {
    //Input sequence of |b*i|^2, indexes of local block, parameters
    //Output: (initial radius)^2
  double clim = 0, ret = 0;
  
    if (mode=='G') {
        //return=(alpha*GH)^2
        clim = lattice_tools::LatticeGH(c+jj-1,bs,INPUT_SQUARED);      // computing from c[jj...jj+bs-1]
        clim *= radius;
        clim *= clim;
        clim = min(clim,to_double(c[jj]));
        ret = clim;
    }
    else if (mode=='0') ret = radius * radius;

    return ret;
}


quad_float ENUMCost(quad_float* c,int jj,int bs,double p0,double radius,char mode,double delta=0,char input=INPUT_SQUARED,char optimize_level=0){
    
    //Predicting enumeration cost
    //c[jj..jj+bs-1]

    progressive_bkz::sharememalloc();
    //Upper bound of # enumeration 

    if (input==INPUT_NONSQUARED) {
        for (int i=jj;i<jj+bs;i++) {
            c[i] *= c[i];
        }
    }
    
    double clim;
    clim = InitRadius(c,jj,bs,radius,mode);     //output is the squared value
    quad_float ret;
    
    //optimize level=0
    //Use set_set_pruning_function_anyprob directly
    if (optimize_level==0) {
        quad_float* V = progressive_bkz::bkz_share_quad_float2[0];
        if (delta==0) {
            delta = Estimate_delta(c+jj,bs); 
        }
        //cout << "delta=" << delta << endl;
        pruning_func::set_volume_factors(V,p0,delta,bs);
        ret = pruning_func::Rigid_upper_cost_pcvf(V,c+jj-1,bs,sqrt(clim));
        //cout << ret << endl;
    }

    //optimize level=1
    //Use set_pruning_function_anyprob with several delta values
    if (optimize_level==1) {
        quad_float* V = progressive_bkz::bkz_share_quad_float2[0];
        if (delta==0) {
            delta = Estimate_delta(c+jj,bs); 
        }
        double d;
        quad_float mincost;
        delta = max(1.005,delta);
        delta = min(1.02,delta);
        
        for (d=max(1.005,delta-0.002);d<min(1.02,delta+0.002);d+=0.0002) {
            pruning_func::set_volume_factors(V,p0,d,bs);
            ret = pruning_func::Rigid_upper_cost_pcvf(V,c+jj-1,bs,sqrt(clim));
            if (mincost==0) mincost = ret;
            mincost = min(mincost,ret);
            //cout << "delta=" << d << " ret=" << ret << endl;
        }
        ret =  mincost;
    }

    //optimize level=2
    //Use set_pruning_function_anyprob and optimizing routine

    if (optimize_level==2) {
       

        quad_float* V = progressive_bkz::bkz_share_quad_float2[0];
        if (delta==0) {
            delta = Estimate_delta(c+jj,bs); 
        }

        pruning_func::init_pruning_func();
        double* R = pruning_func::gnr_share_double[2];
        delta = Estimate_delta(c+jj,bs); 
        delta = max(1.005,delta);
        delta = min(1.02,delta);

        double d,mind = 0;
        quad_float mincost;
        for (d=max(1.005,delta-0.002);d<min(1.02,delta+0.002);d+=0.0002) {
            pruning_func::set_volume_factors(V,p0,d,bs);
            ret = pruning_func::Rigid_upper_cost_pcvf(V,c+jj-1,bs,sqrt(clim));
            if (mincost==0) {
                mincost = ret;
                mind = d;
            }
            if (mincost > ret) {
                  mincost = ret;
                  mind = d;
            }
            //cout << "delta=" << d << " ret=" << ret << endl;
        }
        pruning_func::set_pruning_function_anyprob(R,p0,mind,bs); 
        //RR cost = optimize_pruning_function(R,sqrt(clim),c+jj-1,bs,p0,VL3,bs/2,orderMI);
        RR cost = pruning_func::optimize_pruning_function(R,sqrt(clim),c+jj-1,bs,p0,VL3,3+bs/20,orderMI);
        ret = to_quad_float(cost);
    }
    
    if (input==INPUT_NONSQUARED) {
        for (int i=jj;i<jj+bs;i++) {
            c[i] = sqrt(c[i]);
        }
    }
    return ret;
}
 


int compute_approx(mat_ZZ& B,int m,int n,quad_float** B1,quad_float** mu,quad_float* b,quad_float* c,mat_ZZ* U) {
    long quit = 0;
    quad_float delta=to_quad_float(0.99);
    int i,j;
    //cout << B << endl;
    for (i = 1; i <=m; i++) {
       for (j = 1; j <= n; j++) {
          conv(B1[i][j], B(i, j));
       }
    }
    for (i = 1; i <= m; i++) {
       b[i] = InnerProduct(B1[i], B1[i], n);
/*
       quad_float temp = to_quad_float(0);
       for (j = 1;j <= m; j++) {
           temp += B1[i][j] * B1[i][j];
       }
  
       cout << "i=" << i << " " << temp << " " << b[i] << endl;
       cout << "i=" << i << " " << sqrt(temp) << " " << sqrt(b[i]) << endl;
 */ 
    }
    //LLL as a preprocess and to compute Gram-Schmidt
    verbose = 0;
    m = ll_LLL_QP(B, U, delta, 0, 0, B1, mu, b, c, m, 1, quit);
    //cout << "m=" << m << endl;
    //for (i=1;i<=m;i++) cout << "c[" << i << "]=" << sqrt(c[i]) << endl;

    return m;
}

void compute_approxRR(mat_ZZ& B,int m,int n,mat_RR& B1,mat_RR& mu,vec_RR& b,vec_RR& c,mat_ZZ* U) {
    long quit = 0;
    RR delta=to_RR(0.99);
    int i,j;
    for (i = 1; i <=m; i++) {
       for (j = 1; j <= n; j++) {
          conv(B1(i,j), B(i, j));
       }
    }
    for (i = 1; i <= m; i++) {
       InnerProduct(b(i),B1(i), B1(i));
    }

    //LLL as a preprocess and to compute Gram-Schmidt
    m = ll_LLL_RR(B, U, delta, 0, 0, B1, mu, b, c, m, 1, quit);
}

void convert_to_double(vector<vector<double> >& d,vector<vector<int> >& l,int offset) {
    
    int i,j;
    d.resize(l.size());
    for (i=0;i<(int)d.size();i++) {
        d[i].resize(l[i].size()-offset);
        for (j=0;j<(int)d[i].size();j++) {
            d[i][j] = l[i][j+offset];
        }
    }
}

template <typename T> double l2norm(vector<T>& d,char sq=0) {
    int i;
    double ret=0;
    for (i=0;i<(int)d.size();i++) {
        ret += (double)d[i]*(double)d[i];
    }
    if (sq==0) {
        return sqrt(ret);
    } else {
        return ret;
    }
}

template <typename T> double l2ip(vector<T>& d,vector<T>& d2) {
    int i;
    double ret=0;
    for (i=0;i<(int)d.size();i++) {
        ret += (double)d[i]*(double)d2[i];
    }
    return ret;
}

void computeorthogonal(std::vector<double>& a,std::vector<double>& b) {

    if (a.size()!=b.size()) return;
    int i;
    double mu = 0;
    double norm = 0;
    for (i=0;i<(int)a.size();i++) {
        mu += a[i] * b[i];
        norm += b[i] * b[i];
    }
    mu /= norm;
    for (i=0;i<(int)a.size();i++) {
        a[i] -= mu * b[i];
    }
}



void Insert(mat_ZZ& L,vec_ZZ& v,int index) {
    int n,m;
    int i,j;
    m = L.NumCols();
    n = L.NumRows();
    //cout << "insert: n=" << n << " m=" << m << endl;

    L.SetDims(n+1,m);
    for (i=n-1;i>=index;i--) {
        L[i+1] = L[i];
    }
    L[index] = v;
    LLL_QP(L,0.999,0,0,1);
    
    for (i=0;i<n+1;i++) {
        //cout << "len[" << i << "]=" << LengthOf(L[i]) << endl;
    }

    j = 0;
    while (LengthOf(L[j])==0) j++;
    //cout << "j=" << j << endl;
    for (i=0;i<=n-j;i++) {
        L[i] = L[i+j];
    }
    for (i=0;i<n+1;i++) {
        //cout << "len2[" << i << "]=" << LengthOf(L[i]) << endl;
    }
    L.SetDims(n,m);
    return;
}

void margevectorset(vectorset& V,vectorset& W) {
    int i;
    for (i=0;i<(int)W.size();i++) {
        V.push_back(W[i]);
    }
}

void distinctvectorset(vectorset& D,int opt=0) {

    if (opt==1) {
        //duplication by symmetry
        int i,j;
        for (i=0;i<(int)D.size();i++) {
            j = 0;
            while (j<(int)D[i].size() && (D[i][j]==0)) j++;
            if ((j<(int)D[i].size()) && (D[i][j]<0)) {
              for (j=0;j<(int)D[i].size();j++) D[i][j] = -D[i][j];
            }
        }
    }
    sort(D.begin(),D.end());
    D.erase(unique(D.begin(),D.end()),D.end());
}


int InsertVector(mat_ZZ& L,vectorset &list,int jj,double clim) {

    int i,j,k;
    int n = L.NumCols();
    if (list.size()<=0) return -1;

    std::vector<std::vector<double> > d;
    convert_to_double(d,list,1);

    std::vector<std::vector<double> > li; //integer of L
    li.resize(n);
    for (i=0;i<n;i++) {
        li[i].resize(n);
        for (j=0;j<n;j++) conv(li[i][j],L[i][j]); 
    }

    std::vector<std::vector<double> > dip;
    dip.resize(d.size());
    for (i=0;i<(int)d.size();i++) dip[i].resize(d[i].size());        //dip[i][j]=<d[i],b*[j]>

    std::vector<std::vector<double> > dnorm;
    dnorm.resize(d.size());
    for (i=0;i<(int)d.size();i++) dnorm[i].resize(d[i].size());        //dnorm[i][j] = |proj_j(d[i])|^2
    
    //Allocating memory
    progressive_bkz::sharememalloc();
    quad_float **mu = progressive_bkz::bkz_share_quad_float3[2];
    quad_float *c = progressive_bkz::bkz_share_quad_float2[3]; // squared lengths of Gramm-Schmidt basis vectors
    GramSchmidt(L,mu,c,L.NumCols());

    //for (i=0;i<10;i++) cout << "c[" << i << "]=" << sqrt(c[i]) << " ";
    //cout << endl;

    double norm,minnorm;
    int minindex = 0;
    
    //Simple insertion
    for (i=0;i<jj;i++) {
        //computing \pi_i(v)
        //i=1 is identity
      for (j=0;j<(int)d.size();j++) {
            //compute dip[j][i]=<d[j],b*[i]>
            dip[j][i] = 0;
            for (k=0;k<(int)d[j].size();k++) dip[j][i] += d[j][k] * li[i][k];        //<d[j].b[i]>
            for (k=0;k<i;k++) dip[j][i] -= to_double(mu[i][k]) * dip[j][k];     //<d[j],b*[i]>
            
            if (i==0) {
                dnorm[j][i] = l2norm(d[j]) * l2norm(d[j]);
            } else {
                dnorm[j][i] = dnorm[j][i-1] - dip[j][i-1]*dip[j][i-1] / to_double(c[i-1]);
            }
        }
    }

    i = jj-1;
        minnorm = -1;
        for (j=0;j<(int)d.size();j++) {
            //norm = dnorm[j][i];
            if (dnorm[j][i]>1.0) {
                norm = sqrt(dnorm[j][i]);
                //cout << "norm[" << j << "," << i << "]=" << norm << endl;
                if (minnorm==-1) {
                    minnorm = norm;
                    minindex = j;
                } else if (minnorm > norm) {
                    minnorm = norm;
                    minindex = j;
                }
            }
        }
        if (minnorm < clim) {
            cout << "insert d[" << minindex << "] at index=" << i << endl;
            cout << "|b*i|: " << sqrt(c[i]) << " -> " << minnorm << "(" << clim << ")" << endl;
            vec_ZZ v;
            v.SetLength(n);
            for (k=0;k<n;k++) v[k] = d[minindex][k];
            cout << v << endl;
            Insert(L,v,i);
            return i;
        }
    

    return -1;
     
     
 }


void gen_hkzlattice(quad_float* c,int n,double alpha) {
    
    //generating |b*i|=alpha*GH(L') for all i
    //output c[i]=|b*i|
    int i,j;


    double CNconstant[] = {0.593208,0.582161,0.561454,0.544344,0.522066,0.502545,0.479628,
0.459819,0.438675,0.413708,0.392483,0.370717,0.344447,0.322574,
0.297318,0.273761,0.249247,0.225483,0.199940,0.173832,0.147417,
0.123425,0.100035,0.074487,0.043089,0.020321,-0.013844,-0.042863,
-0.068204,-0.093892,-0.124345,-0.151097,-0.183912,-0.214122,
-0.241654,-0.274612,-0.302966,-0.330965,-0.367514,-0.391956,
-0.426507,-0.457813,-0.488113,-0.518525,-0.554184,-0.585479,
-0.617705,-0.646749,-0.671864,-0.687300};    
    
    for (i=1;i<=min(50,n);i++) {
        c[i] = exp(CNconstant[50-i]);
    }
    
    for (i=51;i<=n;i++) {
        c[i] = 0;
        for (j=1;j<i;j++) {
            c[i] += 1.0*log(c[j]) / (i-1.0);
        }
        c[i] += 1.0*log(alpha)*i/(i-1.0);
        c[i] -= to_quad_float(1.0*log(VolumeBall(i,1.0))/(i-1.0));
        c[i] = exp(c[i]);
    }

    for (i=1;i<=n/2;i++) swap(c[i],c[n-i+1]);
    for (i=2;i<=n;i++) c[i] /= c[1];
    c[1] = 1.0;
    
}

RR minimum_cost(int n,double alpha) {

    int i;
    quad_float* c;
    c = new quad_float[n+1];
    quad_float ucost;
    
    gen_hkzlattice(c,n,1.00);

    for (i=1;i<=n;i++) c[i] *= c[i];    //c[i]=|b*_i|^2
    
    return lattice_tools::FullENUMCost(c,n);
    //ucost = 2.0*ENUMCost(c,1,n,prob,alpha,'G');
    //return ucost;
}

double compute_target_r(int beta) {
    double r;
    lattice_tools::initialize();
    if (beta<=90) {
        r = -18.2139/(beta+318.978);
        r = exp(r);
        if (beta<=35) {
            r /= exp(2.0*log(lattice_tools::smallghconst[beta])/beta);
            //cout << r << endl;
        }    
    } else {
        r = max(-1.06889/(beta-31.0345)*log(0.417419*beta-25.4889), -18.2139/(beta+318.978));
        r = exp(r);
    }
    return r;
}

#include "pbkzsimulator.cpp"



#endif
