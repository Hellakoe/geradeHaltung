//GEN-LINE:variables
package ui;

import java.util.ArrayList; 
import java.util.List;
import javax.swing.JButton;

public class MainFrame extends javax.swing.JFrame {
    private List<String> words;
    /** Creates new form MainFrame */
    public MainFrame() {
        initComponents();
        words = new ArrayList<String>();
    }
    
    private void initComponents() 
    {
        JButton jButton1 = new javax.swing.JButton();
        JButton jButton2 = new javax.swing.JButton();
        JButton jButton3 = new javax.swing.JButton();
        JButton jButton4 = new javax.swing.JButton();
        JButton jButton5 = new javax.swing.JButton();
        JButton jButton6 = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        getContentPane().setLayout(new java.awt.FlowLayout());

        jButton1.setText("jButton1");
        getContentPane().add(jButton1);

        jButton2.setText("jButton2");
        getContentPane().add(jButton2);

        jButton3.setText("jButton3");
        getContentPane().add(jButton3);

        jButton4.setText("jButton4");
        getContentPane().add(jButton4);

        jButton5.setText("jButton5");
        getContentPane().add(jButton5);

        jButton6.setText("jButton6");
        getContentPane().add(jButton6);

        pack();
    }                    

    public static void main(String args[]) {
        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new MainFrame().setVisible(true);
            }
        });
    }
}