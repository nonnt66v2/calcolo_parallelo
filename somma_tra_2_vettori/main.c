//
// Created by nonnt66 on 04/04/24.
//
#include <stdio.h>
#include <omp.h>
#include <math.h>

//

int main(){
    int i,j,h,N=13,r,id,step=0,t,nloc,indice;
    int p = 4;
    int h_end;
    int vet1[13]={2,3,5,6,7,8,3,2,44,6,7,8,9},
            vet2[13]={2,3,5,6,7,8,3,2,44,6,7,8,9},
            vet3[13];
#pragma omp parallel  private(nloc,i,indice),shared(vet1,vet2,vet3,N,p,r), num_threads(4)
    {
//        h = omp_get_thread_num()*(N/p);
//        h_end = (omp_get_thread_num()+1)*(N/p);
//
//        for(j = h;j< h_end;j++){
//            vet3[j] = vet1[j] + vet2[j];
//        }
        t = omp_get_num_threads();
        nloc = N/p;
        r = N%t;
        if(omp_get_thread_num()<r){
            nloc++;
            step=0;
        }
        else{
            step = r;
        }
        for(i = 0;i<nloc;i++){
            indice = i+nloc*omp_get_thread_num()+step;
            vet3[indice] = vet1[indice] + vet2[indice];

        }
        printf("sono %d, di %d numeri %d\n",omp_get_thread_num(),t,nloc);

    }

    for(i=0;i<13;i++){
        printf("%d\n",vet3[i]);
    }
}
