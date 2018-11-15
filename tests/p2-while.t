int main(){
	int i, j;
	i = 0;
	j = 5;
	while(i < 9){
		if(i == 3)
			out i;
		else while(j > 1){
			out j;
			j = j-1;
		}
		i = i+1;
		if(j == 1) continue;
		else ;
	}
	while(i < 15){
		i = i+1;
		if(i == 12){
			out 42;
			break;
		}
		else continue;
		out -42;
	}
	while(0) out 1;
	return i;
}
