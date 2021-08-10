import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.util.Scanner;
import java.rmi.NotBoundException;

public class Client {

	CounterInterface counter;
    
    static Scanner scanner = new Scanner(System.in);

	static String input;

    public Client() {
        if(System.getSecurityManager() == null) {
            System.setSecurityManager(new SecurityManager());
        }

        try {
            counter =(CounterInterface) Naming.lookup("rmi://127.0.0.1/AtomicCounter");
    		
        } catch (final Exception e) {
			System.out.println(e.getMessage());
		}
    }
    
    public void execute() {
    	System.out.println("Digite get para ver o valor do contador ou qualquer outra coisa para sair");
        while (true) {
			input = scanner.next();
			if (input.contentEquals("get")) {
				try {
					System.out.println(counter.getCount());
				} catch (RemoteException e) {
					e.printStackTrace();
				}
			} else {
                System.exit(0);
			}
		}
    }

    public static void main(String[] args) {
        Client c = new Client();
        c.execute();
    }

}
