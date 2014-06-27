#ifndef __PROGTEST__
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;
#endif /* __PROGTEST__ */

class CIndexException{
	friend ostream& operator<<(ostream&,const CIndexException&);
	int  i;
	int ii;
public:
	CIndexException(int ind,int ind2):i(ind),ii(ind2){}
};
class CSizeException{
	friend ostream& operator<<(ostream&, const CSizeException&);
	int a_r,a_c;
	int b_r,b_c;
	char c;
public:
	CSizeException(int ar,int ac,int br,int bc,char ch):a_r(ar),a_c(ac),b_r(br),b_c(bc),c(ch){}
};
class CMatrix{
	int rows,cols;
	double** matrix;
public:
	class Proxy {
		double* _array;
      int   ix1; 
      int rows;
      int cols;
   public:
      Proxy(double* _array, int ix,int row,int col):_array(_array), ix1(ix),rows(row),cols(col){}
		double& operator[](int ix2){
			if((ix1<0 || ix1>rows-1) || (ix2<0 || ix2>cols-1))
				throw CIndexException(ix1,ix2);
         return _array[ix2];
      }
      double operator[](int ix2)const{
         return _array[ix2];
      }
   };
	Proxy operator[](int index) {
      return Proxy(matrix[index], index,rows,cols);
   }
   Proxy operator[](int index)const{
      return Proxy(matrix[index], index,rows,cols);
   }
   ~CMatrix(){
   	for(int i=0;i<rows;i++){
   		delete [] matrix[i];
   		matrix[i]=NULL;
   	}
   	delete [] matrix;
   	matrix=NULL;
   }
   CMatrix(const CMatrix& mat):rows(mat.rows),cols(mat.cols){
   		matrix=new double*[rows];
   		for(int i=0;i<rows;i++){
   		matrix[i]=new double [cols];
   		for(int c=0;c<cols;c++)
   			this->matrix[i][c]=mat[i][c];
   		}
   }
	CMatrix(int r=0, int c=0): rows(r),cols(c){
		matrix = new double*[rows];
		for(int i=0;i<rows;i++){
			matrix[i]=new double[cols];
			for(int c=0;c<cols;c++)
				matrix[i][c]=0;
		}
	}
 	friend istream& operator>>(istream& i, CMatrix& mat);
	friend ostream& operator<<(ostream& os, const CMatrix& mat);
	friend CMatrix  operator+(const CMatrix& a,const CMatrix& b);
	friend CMatrix  operator*(const CMatrix& a,const CMatrix& b);
	friend CMatrix  operator-(const CMatrix& a,const CMatrix& b);
	friend CMatrix  operator-(const CMatrix& a);
	friend CMatrix  operator*(const CMatrix& a,const double num);
	void operator=(const CMatrix& b){
		/*	---------------------------------------------------------------------------------------------------DELETING A SMALL MATRIX*/
		if(rows!=b.rows || cols!=b.cols){
			for(int i=0;i<rows;i++){
	   		delete [] matrix[i];
	   		matrix[i]=NULL;
	   	}
	   	delete [] matrix;
	   	matrix=NULL;
	   /*--------------------------------------------------------------------------------------------------------------------DELETED*/
	   	matrix=new double*[b.rows];
	   	for(int i=0;i<b.rows;i++)
	   		matrix[i]=new double [b.cols];
	   	rows=b.rows;
	   	cols=b.cols;
	   /*----------------------------------------------------------------------------------------------------------------REALLOCATED*/
		}
		for(int i=0;i<rows;i++)
		for(int c=0;c<cols;c++)
			matrix[i][c]=b[i][c];
	}
};
double to_double(const char* str){
	double i;
   	istringstream ss(str);
	ss >> i;
	//cout<<"toDoble "<<i<<endl;
	return i;
}
CMatrix 	operator*(const CMatrix& a,const double num){
	CMatrix tmp(a);
	for(int i=0;i<a.rows;i++)
	for(int c=0;c<a.cols;c++){
		tmp[i][c]=a[i][c]*num;
	}
	return tmp;
}
CMatrix 	operator+(const CMatrix& a,const CMatrix& b){
	if(a.rows!=b.rows || a.cols!=b.cols)
		throw CSizeException(a.rows,a.cols,b.rows,b.cols,'+');
	CMatrix tmp(a);
	for(int i=0;i<a.rows;i++)
	for(int c=0;c<a.cols;c++){
		tmp[i][c]=a[i][c]+b[i][c];
	}
	return tmp;
}
CMatrix 	operator*(const CMatrix& a,const CMatrix& b){
	CMatrix tmp(a.rows,b.cols);
	if( a.cols!=b.rows)
		throw CSizeException(a.rows,a.cols,b.rows,b.cols,'*');
   else{int result;
      for (int i=0;i<a.rows;i++){
         for(int j=0;j<b.cols;j++){
				result = 0;
					for(int k=0;k<b.rows;k++){
               	result += a[i][k]*b[k][j];
            	}
            tmp[i][j] = result;
         }
      }
   }
   return tmp;
}
CMatrix 	operator-(const CMatrix& a,const CMatrix& b){
	CMatrix tmp(a);
	for(int i=0;i<a.rows;i++)
	for(int c=0;c<a.cols;c++){
		tmp[i][c]=a[i][c]-b[i][c];
	}
	return tmp;
}
CMatrix 	operator-(const CMatrix& a){
	CMatrix tmp(a);
	for(int i=0;i<a.rows;i++)
	for(int c=0;c<a.cols;c++){
		tmp[i][c]=-a[i][c];
	}
	return tmp;
}
ostream& operator<<(ostream &os, const CMatrix& mat){
		os<<"{"<<endl<<"   ";
		for(int i=0;i<mat.rows;i++){
			os<<"{";
			for(int c=0;c<mat.cols;c++){
				if(c!=mat.cols-1)
					os<<mat[i][c]<<", ";
				else{
					if(i!=mat.rows-1)
						os<<mat[i][c]<<"},"<<endl;
					else
						os<<mat[i][c]<<"}"<<endl;
				}
			}
		if(i!=mat.rows-1)
			os<<"   ";
		}
		os<<"}"<<endl;
	return os;
}
ostream& operator<<(ostream& os, const CIndexException& ex){
	os<<"Invalid index ["<<ex.i<<"]["<<ex.ii<<"]"<<endl;
	return os;
}
ostream& operator<<(ostream& os, const CSizeException & ex){
	os<<"Invalid matrix size "<<ex.a_r<<'x'<<ex.a_c<<" "<<ex.c<<" "<<ex.b_r<<'x'<<ex.b_c<<endl;
	return os;
}
istream& operator>>(istream& is, CMatrix& mat){
	char in[5000];
	int ind_in=0;
	int amount;
	int rows=0;
	int cols=0;
	int l,r;
	l=r=0;
	while(is.good()){
		is>>skipws>>in[ind_in];
		if(in[ind_in]=='}'){
			rows++;
			r++;
		}
		if(in[ind_in]=='{')
			l++;
		ind_in++;
	}
	if(l!=r){
		is.setstate(ios::failbit);
		return is;
	}

	amount=ind_in-2;
	 // for(int p=0;p<=amount;p++)
	 // 	cout<<in[p]<<' ';
	 // cout<<endl;
	if(amount <3 || in[0]!= '{' || in[1]!='{'){
		// cout<<"nachalo\n";
		is.setstate(ios::failbit);
		return is;
	}

	if(in[amount]!='}' || in[amount-1]!='}'){
		// cout<<"konec\n";
		is.setstate(ios::failbit);
		return is;
	}
	rows--;
	//cout<<"rows= "<<rows<<endl;
	int i=2;
	if(in[i]<'0' || in[i]>'9'){
		is.setstate(ios::failbit);
		return is;
	}		

	while(in[i]!='}'){
		if(in[i]==','){
		 	if(in[i-1]<'0' || in[i-1]>'9' ){
				is.setstate(ios::failbit);
				return is;
			}
			else{
				cols++;
				//cout<<in[i-1]<<' ';
			}
		}
		i++;
	}cols+=1;
	//cout<<"cols= "<<cols<<endl;
	
		double* arr=new double[amount+1];
		int i_arr=0;
		i=2;
		char arr2[10];
		int i_arr2=0;
		int ccol=0;
		int crow=0;
		int flag=0;
		while(i<amount){
			if(in[i]>='0' && in[i]<='9'){
				arr2[i_arr2]=in[i];
				i_arr2++;i++;
				while((in[i]>='0' && in[i]<='9') || in[i]=='.'){
					arr2[i_arr2]=in[i];
					i++;i_arr2++;
				}
				if(in[i]!=',' && in[i]!='{' && in[i]!= '}'){
					//cout<<"posle cisla\n";
					is.setstate(ios::failbit);
					delete [] arr;
						return is;
				} 
				arr[i_arr]=to_double(arr2);
				ccol++;i_arr++;
				while(i_arr2>0){
					arr2[i_arr2]='\0';
					i_arr2--;
				}
				if(in[i]==','){
					i++;
					continue;
				}
																	//"{ { 1, 2, 3, 4 }, { 4, 3, 2, 1 } } "
				if(in[i]=='}'){
					if(ccol!=cols){
						//cout<<"msmatch cols\n";
						is.setstate(ios::failbit);
						delete [] arr;
						return is;
					}
					ccol=0;
					crow++;
					flag=1;
					if(crow>rows){
						//cout<<"crow>rows\n";
						is.setstate(ios::failbit);
						delete [] arr;
						return is;
					}
					if(crow==rows && i!=amount-1){
						//cout<<"crow==rows,but i!=amount-1\n";
						is.setstate(ios::failbit);
						delete [] arr;
						return is;
					}
					if(in[i+1]>='0' && in[i+1]<='9'){
						//cout<<"shit\n";
						is.setstate(ios::failbit);
						delete [] arr;
						return is;
					}
					i++;
					continue;
				}
				if(in[i]=='{' && (!flag || in[i-1]!=',')){
					is.setstate(ios::failbit);
					delete [] arr;
					return is;
				}
				if(in[i]=='{')
					flag=0;
				i++;
				continue;
			}
			if(in[i]=='}'){
				if(ccol!=cols){
					is.setstate(ios::failbit);
					delete [] arr;
					return is;
				}
				ccol=0;
				crow++;
				flag=1;
				if(crow>rows){
					is.setstate(ios::failbit);
					delete [] arr;
					return is;
				}
				if(crow==rows && i!=amount-1){
					is.setstate(ios::failbit);
					delete [] arr;
					return is;
				}
				if(in[i+1]>='0' || in[i+1]<='9'){
					is.setstate(ios::failbit);
					delete [] arr;
					return is;
				}
				i++;
				continue;
			}
			if(in[i]==',' && (in[i+1]==',' || in[i-1]==',')){
				is.setstate(ios::failbit);
				delete [] arr;
				return is;
			}
			if(in[i]=='{' && (!flag || in[i-1]!=',')){
				is.setstate(ios::failbit);
				delete [] arr;
				return is;
			}
			if(in[i]=='{')
				flag=0;
			i++;
		}
	 is.clear();
	 //-------------------------------------------------------------------DELETING A MATRIX
	for(int w=0;w<mat.rows;w++){
		delete [] mat.matrix[w];
		mat.matrix[w]=NULL;
	}
	delete [] mat.matrix;
	mat.matrix=NULL;
	//-----------------------------------------------------------------------------DELETED
	mat.matrix=new double*[rows];
	for(int w=0;w<rows;w++)
	   mat.matrix[w]=new double [cols];
	mat.rows=rows;
	mat.cols=cols;
	//-------------------------------------------------------------------------REALLOCATED
	int z=0;
	for(int r=0;r<rows;r++)
	for(int c=0;c<cols;c++){
		mat[r][c]=arr[z];
		z++;
	}
	delete [] arr;
	return is;
}

#ifndef __PROGTEST__

int main(void){	
	istringstream is;
	ostringstream os;
	double x;
	CMatrix a ( 2, 3 );
	a[0][0] = 1;
	a[0][1] = 2;
	a[0][2] = 3;
	a[1][0] = 4;
	a[1][1] = 5;
	a[1][2] = 6;
	const CMatrix b = a;
	CMatrix c ( 3, 2 );
	c[0][0] = 1;
	c[0][1] = 1;
	c[1][0] = 2;
	c[1][1] = -2;
	c[2][0] = 3;
	c[2][1] = 3;
	os . str ("");
	os<<a;
	cout<<"BBBBBBBBBBBBB--8<----8<----8<----8<----8<--"<<endl;
	os . str ("");
	os << b;
	cout<<"CCCCCCCCCCCCC--8<----8<----8<----8<----8<--"<<endl;
	os . str ("");
	os << c;
	cout<<"DDDDDDDDDDDDD--8<----8<----8<----8<----8<--"<<endl;
	CMatrix d ( 1, 1 );
	d[0][0] = -1;
	os . str ("");
	os << d;
	cout<<"DDDD=AAA+BBB--8<----8<----8<----8<----8<--"<<endl;
	d = a + b;
	os . str ("");
	os << d;
	cout<<"DDDDD=AAA-BBB--8<----8<----8<----8<----8<--"<<endl;	
	d = a - b;
	os . str ("");
	os << d;
	cout<<"DDDD=-AAAAA--8<----8<----8<----8<----8<--"<<endl;	
	d = - a;
	os . str ("");
	os << d;
	cout<<"DDDD=AAAA*2--8<----8<----8<----8<----8<--"<<endl;	
	d = a * 2;
	os . str ("");
	os << d;
	x=d[1][1];
	cout<<"x= "<<x<<endl;
	cout<<"DDDD=BBB*CCC--8<----8<----8<----8<----8<--"<<endl;
	d = b * c;
	os . str ("");
	os << d;
	cout<<"--8<----8<----8<----8<----8<--"<<endl;
	x=b[0][0];
	cout<<x<<endl;
	try{
		d=b+c;
	}
	catch(const CSizeException & e){
		cout <<e;
	}
	cout<<"--8<----8<----8<----8<----8<--"<<endl;
	is . str ( "{ { 1, 2 },, { 3, 4 } }" );
	is>>a;
	cout<<"failbit= "<<is.fail()<<endl;
	d=a;
	os.str("");
	os<<a;
	is.clear();
	is . str ( "{ { 1, 2, 3 } { 3, 4, 5} } " );
	is>>c;
	cout<<"failbit= "<<is.fail()<<endl;
	return 0;
}
#endif /* __PROGTEST__ */
