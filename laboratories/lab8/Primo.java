/* Aluno: Andrew da Silva Faria */
/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 8 */
/* Codigo: Criando um pool de threads em Java */

/*
OBS: Para executar o programa devemos ter na linha de comando:
    java Primo N
Onde N é o tamanho da sequência que será usada para buscarmos o número de primos
*/

import java.util.ArrayList;
import java.util.LinkedList;
import java.lang.Math;
import java.util.List;

//-------------------------------------------------------------------------------
//Classe que define um pool de threads 
class FilaTarefas {
    private final int nThreads;
    private final MyPoolThreads[] threads;
    private final LinkedList<Runnable> queue;
    private boolean shutdown;

    public FilaTarefas(int nThreads) {
        this.shutdown = false;
        this.nThreads = nThreads;
        queue = new LinkedList<Runnable>();
        threads = new MyPoolThreads[nThreads];
        for (int i=0; i<nThreads; i++) {
            threads[i] = new MyPoolThreads();
            threads[i].start();
        } 
    }

    public void execute(Runnable r) {
        synchronized(queue) {
            if (this.shutdown) return;
            queue.addLast(r);
            queue.notify();
        }
    }
    
    public void shutdown() {
        synchronized(queue) {
            this.shutdown=true;
            queue.notifyAll();
        }
        for (int i=0; i<nThreads; i++) {
            try { threads[i].join(); } catch (InterruptedException e) { return; }
        }
    }

    private class MyPoolThreads extends Thread {
        public void run() {
            Runnable r;
            while (true) {
                synchronized(queue) {
                    while (queue.isEmpty() && (!shutdown)) {
                        try { queue.wait(); }
                        catch (InterruptedException ignored){}
                    }
                    if (queue.isEmpty() && shutdown) return;  
                    r = (Runnable) queue.removeFirst();
            }
            try { r.run(); }
            catch (RuntimeException e) {}
            } 
        } 
    } 
}

//-------------------------------------------------------------------------------

// Classe para termos um objeto que atua como contador
class Count {
    private long count;
    
    public Count(){
        this.count = 0; 
    }

    public synchronized void inc(){
        this.count += 1;
    }

    public long getCount(){
        return this.count;
    }
}

//--PASSO 1: cria uma classe que implementa a interface Runnable
class ehPrimo implements Runnable {
    long n;
    Count C;

    public ehPrimo(long n, Count C) { 
        this.n = n;
        this.C = C;
    }

    //--metodo executado pela thread
    public void run() {
        int i;
        if (n <= 1) 
            return;
        if (n == 2) 
            C.inc();
        if (n % 2 == 0) 
            return;
        for (i = 3; i < Math.sqrt(n)+1; i += 2)
            if(n % i == 0) 
                return;
        C.inc();
    }
}

// Classe para verificacao da corretude
class PrimoSequencial {
    public static long numeroPrimos(long N) {
        long quant = 0;

        for(int n = 1; n <= N; n++) {
            int i;
            if (n <= 1) 
                continue;
            else if (n == 2) {
                quant++;
                continue;
            }
            else if (n % 2 == 0) 
                continue;
            else {
                for (i = 3; i < Math.sqrt(n)+1; i += 2)
                    if(n % i == 0) 
                        break;
                if(i >= Math.sqrt(n) + 1)
                    quant++;
            }
        }
        return quant;
    }
}


//Classe da aplicação (método main)
class Primo {
    private static final int NTHREADS = 10;

    public static void main (String[] args) {

        long n = 0;

        // Verifica se temos o argumento da quantidade de valores na sequencia e armazena na variavel `n`
        if (args.length != 1) {
            System.out.println("Usage: java programa numeroDaSequencia");
            return;
        }
        else {
            try {
                n = Long.parseLong(args[0]);
            } catch (NumberFormatException e) {
                System.err.println("Argument " + args[0] + " must be an integer.");
                System.exit(1);
            }
        }
        
        // cria o pool de threads
        FilaTarefas pool = new FilaTarefas(NTHREADS); 
        List<Count> lista_contadores = new ArrayList<Count>();
        int j = 0;
        for (int i = 0; i < NTHREADS; i++){
            Count C = new Count();
            lista_contadores.add(C);
        }
      
        // dispara a execução dos objetos runnable usando o pool de threads
        for (long i = 1; i <= n; i++, j = (j+1) % NTHREADS) {
            Runnable r = new ehPrimo(i, lista_contadores.get(j));
            pool.execute(r);
        }

        // esperar pelo termino das threads
        pool.shutdown();

        int soma = 0;
        for(int i = 0; i < NTHREADS; i++){
            Count C = lista_contadores.get(i);
            soma += C.getCount();
        }

        System.out.println("A quantidade de números primos de 1 a "+ n + " é: " + soma);

        //Descomentar para ver a verificacao de numeros primos encontrados usando o algoritmo sequencial
        //System.out.println("A quantidade de números primos usando o algoritmo sequencial é: " + PrimoSequencial.numeroPrimos(n));
    }
}
