package unibo.btclient;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.UUID;
import java.util.stream.Collectors;

import unibo.btlib.BluetoothChannel;
import unibo.btlib.BluetoothUtils;
import unibo.btlib.ConnectToBluetoothServerTask;
import unibo.btlib.ConnectionTask;
import unibo.btlib.RealBluetoothChannel;
import unibo.btlib.exceptions.BluetoothDeviceNotFound;
import unibo.btclient.utils.C;

public class MainActivity extends AppCompatActivity {
    private BluetoothChannel btChannel;
    private BluetoothAdapter adapter = null;
    private Set<BluetoothDevice> Devices;
    private UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");


    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final BluetoothAdapter btAdapter = BluetoothAdapter.getDefaultAdapter();
        if (btAdapter != null && !btAdapter.isEnabled()) {
            startActivityForResult(
                    new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE),
                    C.bluetooth.ENABLE_BT_REQUEST
            );
        }

        initUI();
    }

    private void initUI() {
        adapter = BluetoothAdapter.getDefaultAdapter();
        this.printami(adapter.getBondedDevices());
        BluetoothDevice hc05 = adapter.getRemoteDevice("00:14:03:05:F2:D9");

        BluetoothSocket socket = null;


        findViewById(R.id.bt_connection_req).setOnClickListener(l -> {
            l.setEnabled(false);
            try {
                pairDevices();
            } catch (BluetoothDeviceNotFound bluetoothDeviceNotFound) {
                Toast.makeText(this, "Bluetooth device not found !", Toast.LENGTH_LONG)
                        .show();
                bluetoothDeviceNotFound.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                l.setEnabled(true);
            }
        });

        findViewById(R.id.sendBtn).setOnClickListener(l -> {
            String message = ((EditText)findViewById(R.id.editText)).getText().toString();
            btChannel.sendMessage(message);
            ((EditText)findViewById(R.id.editText)).setText("");
        });
    }

    private void pairDevices() {
        Devices = adapter.getBondedDevices();
        List<String> list = new ArrayList();


        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N && Devices.size() > 0) {
            Devices.forEach(el -> list.add(el.getName() + " - " + el.getAddress()));
        }
        else {
            // In case no device is found
            Toast.makeText(getApplicationContext(), "No Paired Bluetooth Devices Found.", Toast.LENGTH_LONG).show();
        }

        // Adding the devices to the list with ArrayAdapter class
        final ArrayAdapter adapter = new ArrayAdapter(this, android.R.layout.simple_list_item_1, list);
        devicelist.setAdapter(adapter);

        // Method called when the device from the list is clicked
        devicelist.setOnItemClickListener(myListListener);
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
        final BluetoothDevice serverDevice = BluetoothUtils
                .getPairedDeviceByName(C.bluetooth.BT_DEVICE_ACTING_AS_SERVER_NAME);
        // !!! Choose the right UUID value
        final UUID uuid = BluetoothUtils.getEmbeddedDeviceDefaultUuid();
//        final UUID uuid = BluetoothUtils.generateUuidFromString(C.bluetooth.BT_SERVER_UUID);

        new ConnectToBluetoothServerTask(serverDevice, uuid, new ConnectionTask.EventListener() {
            @Override
            public void onConnectionActive(final BluetoothChannel channel) {
                ((TextView) findViewById(R.id.statusLabel)).setText(String.format(
                    "Status : connected to server on device %s",
                    serverDevice.getName()
                ));

                findViewById(R.id.connectBtn).setEnabled(false);

                btChannel = channel;
                btChannel.registerListener(new RealBluetoothChannel.Listener() {
                    @Override
                    public void onMessageReceived(String receivedMessage) {
                        ((TextView) findViewById(R.id.chatLabel)).append(String.format(
                            "> [RECEIVED from %s] %s\n",
                            btChannel.getRemoteDeviceName(),
                            receivedMessage
                        ));
                    }

                    @Override
                    public void onMessageSent(String sentMessage) {
                        ((TextView) findViewById(R.id.chatLabel)).append(String.format(
                                "> [SENT to %s] %s\n",
                                btChannel.getRemoteDeviceName(),
                                sentMessage
                        ));
                    }
                });
            }

            @Override
            public void onConnectionCanceled() {
                ((TextView) findViewById(R.id.statusLabel)).setText(String.format(
                    "Status : unable to connect, device %s not found!",
                    C.bluetooth.BT_DEVICE_ACTING_AS_SERVER_NAME
                ));
            }
        }).execute();
    }*/
    private void printami(Object obj){
        System.out.println(obj.toString());
    }
}
