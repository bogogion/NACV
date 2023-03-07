import java.io.*;
import java.net.*;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/* Default connection type is UDP. */

public class CommClient implements Runnable
{
    private Thread t;
    /* Create our sockets  */ 
    private DataInputStream m_input   = null;
    private DatagramSocket  m_socket  = null;
    private DatagramPacket  m_receive = null;
    byte[] receive_buf = new byte[9];

    private float  dist;
    private byte   id;
    private float  angle;
    private int    port;

    public CommClient(int port)
    {
        this.port = port;
    }

    /* Binds to port using UDP and sets up the packets*/
    public void connectClient(int port)
    {
        try {
            m_socket = new DatagramSocket(port);
            m_receive = new DatagramPacket(receive_buf, 9);
            m_input = new DataInputStream(new ByteArrayInputStream(m_receive.getData(), m_receive.getOffset(), m_receive.getLength()));
        } catch (Exception i) {
            System.out.println(i);
        } 
    }

    public void receiveMessage()
    {
	try {
        m_socket.receive(m_receive);

        try {
            m_input.read(receive_buf, 0, 9);
        } catch (EOFException e) {
            System.out.print(e);
        } 

        this.id = receive_buf[0];

	    /* Grab the 1st float value */
	    this.dist = ByteBuffer.wrap(receive_buf).order(ByteOrder.BIG_ENDIAN).getFloat(1);

	    /* Grab the 2nd float value */
	    this.angle = ByteBuffer.wrap(receive_buf).order(ByteOrder.BIG_ENDIAN).getFloat(5);
	} catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void printMessage()
    {
 	    System.out.println(this.id);	
	    System.out.println(this.dist);
	    System.out.println(this.angle);
    }

    public void run()
    {
        System.out.println("Thread ran!");
        connectClient(port);

        while(true)
        {
            receiveMessage();
            printMessage();
        }
    }

    public void start(String name)
    {
        if(t == null)
        {
            System.out.println("t = null");
            t = new Thread(this,name);
            t.start();
        }
    }

    public static void main(String args[]) throws EOFException
    {
        CommClient client = new CommClient(5805);
        client.start("ClientThread");
    }
}   
