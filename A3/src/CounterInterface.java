import java.rmi.Remote;
import java.rmi.RemoteException;

public interface CounterInterface extends Remote{
	public int getCount() throws RemoteException;
}
