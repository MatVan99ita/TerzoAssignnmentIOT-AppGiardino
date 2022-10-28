package unibo.btclient;

import android.app.AlertDialog;
import android.app.Dialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.constraint.ConstraintLayout;
import android.support.v7.app.AppCompatActivity;
import android.text.Layout;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.util.UUID;

import unibo.btlib.BluetoothChannel;
import unibo.btlib.BluetoothUtils;
import unibo.btlib.ConnectToBluetoothServerTask;
import unibo.btlib.ConnectionTask;
import unibo.btlib.RealBluetoothChannel;
import unibo.btlib.exceptions.BluetoothDeviceNotFound;
import unibo.btclient.utils.C;

public class MainActivity extends AppCompatActivity {
    private BluetoothChannel btChannel;
    private BluetoothAdapter btAdapter = BluetoothAdapter.getDefaultAdapter();
    private UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private UUID u = UUID.randomUUID();
    private BluetoothDevice hc05 = btAdapter.getRemoteDevice("00:14:03:05:F2:D9");
    private boolean ledSwitch = false;
    private BluetoothSocket socket = null;

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ConstraintLayout layout = findViewById(R.id.layout_brutto);
        for (int i = 0; i < layout.getChildCount(); i++) {
            View child = layout.getChildAt(i);
            child.setEnabled(false);
        }
        Button btButton = layout.findViewById(R.id.bt_connection_req);
        btButton.setEnabled(true);
        final BluetoothAdapter btAdapter = BluetoothAdapter.getDefaultAdapter();
        if (btAdapter != null && !btAdapter.isEnabled()) {
            startActivityForResult(
                    new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE),
                    C.bluetooth.ENABLE_BT_REQUEST
            );
        }

        initUI(layout);
    }

    private void initUI(ConstraintLayout layout) {

        findViewById(R.id.bt_connection_req).setOnClickListener(l -> {
            l.setEnabled(false);
            try {
                pairDevices();
            } catch (IOException | BluetoothDeviceNotFound e) {
                e.printStackTrace();
            } finally {
                l.setEnabled(true);
                Button led1 = layout.findViewById(R.id.led1);
                led1.setEnabled(true);
            }
        });
        findViewById(R.id.led1).setOnClickListener(l -> {
            switchLed();
        });

    }

    private void switchLed() {

        createDialog("LEDDAMI");
        if(ledSwitch){
            btChannel.sendMessage("0");
        } else {
            btChannel.sendMessage("1");
        }
        ledSwitch = !ledSwitch;
    }

    private void pairDevices() throws IOException, BluetoothDeviceNotFound {
        if(socket == null || !socket.isConnected()){
            this.socket = hc05.createInsecureRfcommSocketToServiceRecord(u);
            this.connectToBTServer();
        }
    }

    @Override
    protected void onStop() {
        super.onStop();
        btChannel.close();
    }

    @Override
    protected void onActivityResult(final int requestCode, final int resultCode,
                                    @Nullable final Intent data) {
        if (requestCode == C.bluetooth.ENABLE_BT_REQUEST && resultCode == RESULT_OK) {
            Log.d(C.APP_LOG_TAG, "Bluetooth enabled!");
        }

        if (requestCode == C.bluetooth.ENABLE_BT_REQUEST && resultCode == RESULT_CANCELED) {
            Log.d(C.APP_LOG_TAG, "Bluetooth not enabled!");
        }
    }

    private void connectToBTServer() throws BluetoothDeviceNotFound {
        final BluetoothDevice serverDevice = BluetoothUtils.getPairedDeviceByName(C.bluetooth.BT_DEVICE_ACTING_AS_SERVER_NAME);
        // !!! Choose the right UUID value
        //final UUID uuid = BluetoothUtils.getEmbeddedDeviceDefaultUuid();
//        final UUID uuid = BluetoothUtils.generateUuidFromString(C.bluetooth.BT_SERVER_UUID);
        createDialog("IO SONO QUA");
        new ConnectToBluetoothServerTask(serverDevice, this.u, new ConnectionTask.EventListener() {
            @Override
            public void onConnectionActive(final BluetoothChannel channel) {
                createDialog("Attivo");
                findViewById(R.id.bt_connection_req).setEnabled(false);

                btChannel = channel;
                btChannel.registerListener(new RealBluetoothChannel.Listener() {
                    @Override
                    public void onMessageReceived(String receivedMessage) {
                        createDialog(String.format(
                                "> [RECEIVED from %s] %s\n",
                                btChannel.getRemoteDeviceName(),
                                receivedMessage
                        ));
                    }

                    @Override
                    public void onMessageSent(String sentMessage) {
                        createDialog(String.format(
                                "> [SENT to %s] %s\n",
                                btChannel.getRemoteDeviceName(),
                                sentMessage
                        ));
                    }
                });
            }

            @Override
            public void onConnectionCanceled() {
                createDialog("Status : unable to connect, device %s not found!");
            }
        }).execute();
    }//*/

    private void createDialog(String testoh){
        Log.d("MainActivity", testoh);
    }
}
