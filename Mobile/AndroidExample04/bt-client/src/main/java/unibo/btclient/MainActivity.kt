package unibo.btclient

import android.support.v7.app.AppCompatActivity
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
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
import android.widget.*
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

    /**Gruppo di bottoni per la luce*/
    private lateinit var btnLed1: Button
    private lateinit var btnLed2: Button
    private lateinit var btnLed3_up: Button
    private lateinit var btnLed3_down: Button
    private lateinit var btnLed4_up: Button
    private lateinit var btnLed4_down: Button

    /**Gruppo di bottoni per l'irrigazione*/
    private lateinit var btnIrrigazione: Button
    private lateinit var btnIrrigazione_fast: Button
    private lateinit var btnIrrigazione_slow: Button

    /**Gruppo di label*/
    private lateinit var led3: TextView
    private lateinit var led4: TextView
    private lateinit var irrigazione: TextView


    private lateinit var btnBell: ImageButton

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        btnLed1 = findViewById(R.id.led1)
        btnLed2 = findViewById(R.id.led2)

        btnLed3_up = findViewById(R.id.led3plus)
        btnLed3_down = findViewById(R.id.led3minus)

        btnLed4_up = findViewById(R.id.led4plus)
        btnLed4_down = findViewById(R.id.led4minus)

        btnIrrigazione = findViewById(R.id.irrigation_onoff)
        btnIrrigazione_fast = findViewById(R.id.irrigation_plus)
        btnIrrigazione_slow = findViewById(R.id.irrigation_minus)
        /*
            * btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                btn.setEnabled(false);

             new Handler().postDelayed(new Runnable() {

                @Override
                public void run() {
                    // This method will be executed once the timer is over
                        btn.setEnabled(true);
                        Log.d(TAG,"resend1");

                }
            },2000);// set time as per your requirement
            }
            });
            *
            * */

        led3 = findViewById(R.id.led3_view)
        led4 = findViewById(R.id.led4_view)
        irrigazione = findViewById(R.id.irrigation_view)

        btnBell = findViewById(R.id.alarm_btn)

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
            }
        })

    }

    /**
     * Trovare un modo per ricordare se i led sono accesi e da lì inviare i comandi di accensione in base al led scelto
     * o usando il nome o il numero della porta
     */
    private fun switchLed(led_num: Int){
        val message: String = ""+led_num
    }

    /**
     * invio della quantità di fade al led specificato
     * @param fade_amount 0 .. 255
     */
    private fun fadeLed(led_num: Int, fade_amount: Int): Int {
        val message: String = ""+led_num+"="
        if(fade_amount < 0) {
            message.plus(0)
        }
        else if(fade_amount > 255) {
            message.plus(255)
        }
        else {
            message.plus(fade_amount)
        }
        return fade_amount
    }

    /**
     * invio dell'attivazione dell'irrigazione
     *
     * (nel controller del bottone va disattivato per n minuti se si ha ancora il controllo)
     */
    private fun irrigationStart(velocity: Int){
        val message: String = ""+velocity
    }

    /**
     * Funzione per l'allarme
     */
    private fun disableAlarm(){

    }




    private fun pairDevices() {
        ConnectToDevice(this).execute()
        var bt_led: Button = findViewById(R.id.led1)
        bt_led?.isEnabled = true
        bt_led.setOnClickListener(View.OnClickListener {
            arduinoCommunication("BELIIIN")
        })
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