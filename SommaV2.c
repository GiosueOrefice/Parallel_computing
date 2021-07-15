#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
        int i,N,t,nloc,r,id,step; /*t serve per il numero di thread, nloc numero di elementi locali di cui si occupera` ogni thread.*/
        float sumtot,sum,*a; 
        sumtot=0;

        printf("Inserire N: ");
        scanf("%d", &N);

        a=(float *)calloc(N,sizeof(float));
        
        printf("Inserire i numeri da sommare: ");
        for(i=0;i<N;i++)
        {
            scanf("%f", &a[i]);
        }
    
        #pragma omp parallel private(nloc,i,id,step) shared(sumtot,r) reduction(+:sum)/*sono private e non first private perche` non ho assegnato nessun valore precedentemente, quindi non sono ancora inizializzate.*/
        {

            t=omp_get_num_threads();
            nloc=N/t; /*viene suddiviso il lavoro tra i vari thread del team, si calcola quanti numeri ogni thread  deve avere. Il numero degli elementi da sommare deve essere un numero divisibile per t. Come si nota non ci sono meccanismi di sincronizzazione, il tutto avviene in maniera sequenziale*/
            r=N%t;
            id=omp_get_thread_num();
            printf("sono %d, di %d: numeri %d, r=%d\n", omp_get_thread_num(),t,nloc,r);
            if(id<r)
            {
                nloc++;
                step=0;
            }
            else step=r;    
            printf("Sono %d, di %d: numeri %d, r=%d, passo=%d\n", id,t,nloc,r,step);
            sum=0;
            for(i=0;i<nloc;i++)
            {
                sum=sum+a[i+nloc*omp_get_thread_num()+step];
            }
            printf("Sono %d, di %d: numeri %d, r=%d, i mia summ=%f\n",id,t,nloc,r,sum);
            sumtot+=sum; /*quando si tenta di accedere ad una variabile condivisa mediante variabili private, l'accesso avviene in maniera sequenziale da parte dei threads proprio per conservare la consistenza dei dati!*/
        }
            printf("somma totale:%f\n",sumtot);
}
