import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject; 

public class Counter extends UnicastRemoteObject implements CounterInterface{
	private int count;

	public Counter() throws RemoteException {
		count = 0;
	}


	@Override
	public synchronized int getCount() throws RemoteException {
		return ++count;
	}
}