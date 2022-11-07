package unibo.btclient

import android.support.v7.app.AppCompatActivity
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.widget.ArrayAdapter
import android.os.Bundle
import unibo.btclient.R
import android.content.Intent
import unibo.btclient.MainActivity
import android.app.Activity
import android.app.ProgressDialog
import android.bluetooth.BluetoothSocket
import android.content.Context
import android.os.AsyncTask
import android.support.constraint.ConstraintLayout
import android.util.Log
import android.view.View
import android.widget.Button
import android.widget.ListView
import unibo.btclient.utils.BluetoothChannel
import unibo.btclient.utils.C
import unibo.btlib.BluetoothUtils
import java.io.IOException
import java.util.*

class MainActivity : AppCompatActivity() {
    companion object {
        private const val BLUETOOTH_ON = 1000
        private val hc05_address: String = "00:14:03:05:F2:D9"
        private val mUUID: UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
        private var btAdapter: BluetoothAdapter = BluetoothAdapter.getDefaultAdapter()
        private var dispositivi: Set<BluetoothDevice> = HashSet()
        private val hc05: BluetoothDevice = btAdapter.getRemoteDevice(hc05_address);
        private lateinit var adapter: ArrayAdapter<Any?>
        private var socket: BluetoothSocket = hc05.createRfcommSocketToServiceRecord(mUUID)
    }

    //private lateinit var btChannel: BluetoothChannel

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        //btAdapter = BluetoothAdapter.getDefaultAdapter()
        //lv = findViewById<View>(R.id.listview) as ListView
        adapter = ArrayAdapter<Any?>(this, android.R.layout.simple_list_item_1)
        //lv!!.adapter = adapter
        var layout: ConstraintLayout = findViewById(R.id.layout_brutto);

        for (el in 0..layout.childCount ){
            val child = layout.getChildAt(el)
            child?.isEnabled = false
        }
        this.scan()
        val btButton: Button = layout.findViewById(R.id.bt_connection_req);
        btButton?.isEnabled = true
        btButton?.setOnClickListener(View.OnClickListener {
            try {
                pairDevices();
            } catch (e: IOException) {
                e.printStackTrace();
            } /*finally {
                l.setEnabled(true);
                Button led1 = layout.findViewById(R.id.led1);
                led1.setEnabled(true);
            }*/
        })

    }

    private fun pairDevices() {
        ConnectToDevice(this).execute()
        var bt_led: Button = findViewById(R.id.led1)
        bt_led?.isEnabled = true
        bt_led.setOnClickListener(View.OnClickListener {
            arduinoCommunication("BELIIIN")
        })
    /*
        if(!socket.isConnected()){

            socket = hc05.createInsecureRfcommSocketToServiceRecord(this.mUUID)
            //BluetoothAdapter.getDefaultAdapter().cancelDiscovery()
            socket!!.connect()
            var bt_led: Button = findViewById(R.id.led1)
            bt_led?.isEnabled = true
            bt_led.setOnClickListener(View.OnClickListener {
                arduinoCommunication()
            })
        }*/
    }

    private fun arduinoCommunication(input: String) {
        if (socket != null) {
            try{
                socket!!.outputStream.write(input.toByteArray())
            } catch(e: IOException) {
                e.printStackTrace()
            }
        }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == BLUETOOTH_ON && resultCode == RESULT_OK) {
            load()
        }
    }

    private fun scan() {
        if (!btAdapter!!.isEnabled) {
            val turnOn = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            startActivityForResult(turnOn, BLUETOOTH_ON)
        }/* else {
            load()
        }*/
    }

    private fun load() {
        dispositivi = btAdapter!!.bondedDevices
        adapter!!.clear()
        for (bt in dispositivi){
            val s: String? = bt.name + " - " + bt.address
            adapter!!.add(s)
        }
    }


    private class ConnectToDevice(var context: Context) : AsyncTask<Void, Void, String>() {
        private var connectSuccess: Boolean = true
        private lateinit var m_progress: ProgressDialog


        override fun onPreExecute() {
            super.onPreExecute()
            m_progress = ProgressDialog.show(context, "Connecting...", "please wait")
        }

        override fun doInBackground(vararg p0: Void?): String? {
            try {
                if (socket == null || !socket.isConnected) {
                    btAdapter = BluetoothAdapter.getDefaultAdapter()
                    val device: BluetoothDevice = btAdapter.getRemoteDevice(hc05_address)
                    socket = device.createInsecureRfcommSocketToServiceRecord(mUUID)
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery()
                    socket!!.connect()
                }
            } catch (e: IOException) {
                connectSuccess = false
                e.printStackTrace()
            }
            return null
        }

        override fun onPostExecute(result: String?) {
            super.onPostExecute(result)
            if (!connectSuccess) {
                Log.i("data", "couldn't connect")
            } else {
                Log.i("data", "BELIN")
            }
            m_progress.dismiss()
        }
    }

    private fun createDialog(testoh: String) {
        Log.d("MainActivity", testoh)
    }


}