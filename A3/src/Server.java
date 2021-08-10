import java.lang.SecurityManager;
import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.RemoteException;

//Executar:
// rmiregistry &
// java -Djava.security.policy=server.policy Server

public class Server {
	
    public Server() {  
		if(System.getSecurityManager() == null) {
		     System.setSecurityManager(new SecurityManager());
		}
		try {  
	        Naming.rebind("AtomicCounter", new Counter());  
	    }  
	    catch (RemoteException | MalformedURLException e) {
            e.printStackTrace();
        }
    }
	
	public static void main(String[] args) {
		new Server();
	}

}
