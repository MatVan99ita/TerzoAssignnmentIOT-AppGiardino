package unibo.btclient

import android.support.v7.app.AppCompatActivity
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.os.Bundle
import android.content.Intent
import android.app.ProgressDialog
import android.bluetooth.BluetoothSocket
import android.content.Context
import android.os.AsyncTask
import android.os.Handler
import android.support.constraint.ConstraintLayout
import android.util.Log
import android.view.View
import android.view.animation.AnimationUtils
import android.widget.*
import java.io.IOException
import java.util.*


/**
 * TODO:
 *      • finire le funzioni per i vari btn
 *      • creare un modo visivo per vedere i led accesi anche dall'app
 *      • sistemare i messaggi da inviare all'arduino
 *
 */

class MainActivity : AppCompatActivity() {
    companion object {
        private const val BLUETOOTH_ON = 1000
        private const val hc05_address: String = "00:14:03:05:F2:D9"
        private val mUUID: UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
        private var btAdapter: BluetoothAdapter = BluetoothAdapter.getDefaultAdapter()
        private var dispositivi: Set<BluetoothDevice> = HashSet()
        private val hc05: BluetoothDevice = btAdapter.getRemoteDevice(hc05_address);
        private lateinit var adapter: ArrayAdapter<Any?>
        private var socket: BluetoothSocket? = null
    }

    /**Gruppo di bottoni per la luce*/
    private lateinit var btnLed1: Button
    private lateinit var btnLed2: Button
    private lateinit var btnLed3up: Button
    private lateinit var btnLed3down: Button
    private lateinit var btnLed4up: Button
    private lateinit var btnLed4down: Button

    /**Gruppo di bottoni per l'irrigazione*/
    private lateinit var btnIrrigazione: Button
    private lateinit var btnIrrigazione_fast: Button
    private lateinit var btnIrrigazione_slow: Button

    /**Gruppo di label*/
    private lateinit var led3: TextView
    private lateinit var led4: TextView
    private lateinit var irrigazione: TextView

    private lateinit var btnBell: ImageButton
    private lateinit var btnConnection: Button

    private var fade_amount1: Int = 0
    private var fade_amount2: Int = 0
    private var isLed1on: Boolean = false
    private var isLed2on: Boolean = false
    private var irrigation_velocity: Int = 0

    private lateinit var layout: ConstraintLayout

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        btnLed1 = findViewById(R.id.led1)
        btnLed2 = findViewById(R.id.led2)

        btnLed3up = findViewById(R.id.led3plus)
        btnLed3down = findViewById(R.id.led3minus)

        btnLed4up = findViewById(R.id.led4plus)
        btnLed4down = findViewById(R.id.led4minus)

        btnIrrigazione = findViewById(R.id.irrigation_onoff)
        btnIrrigazione_fast = findViewById(R.id.irrigation_plus)
        btnIrrigazione_slow = findViewById(R.id.irrigation_minus)
        btnConnection = findViewById(R.id.bt_connection_req)
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

        led3.setText(fade_amount1.toString())
        led4.setText(fade_amount2.toString())
        irrigazione.setText(irrigation_velocity.toString())


        btnBell = findViewById(R.id.alarm_btn)

        //btAdapter = BluetoothAdapter.getDefaultAdapter()
        //lv = findViewById<View>(R.id.listview) as ListView
        adapter = ArrayAdapter<Any?>(this, android.R.layout.simple_list_item_1)
        //lv!!.adapter = adapter

        layout = findViewById(R.id.layout_brutto);
        for (el in 0..layout.childCount ){
            val child = layout.getChildAt(el)
            child?.isEnabled = false
        }
        this.scan()
        btnConnection.isEnabled = true

        this.addClickEvent()
    }



    /**
     * Funzione per assegnare ai vari bottoni la propria funzione
     */
    private fun addClickEvent() {
        btnLed1.setOnClickListener(View.OnClickListener {
            switchLed(1, btnLed1)
        })
        btnLed2.setOnClickListener(View.OnClickListener {
            switchLed(1, btnLed2)
        })

        /*btnLed1.setOnClickListener(View.OnClickListener {
            arduinoCommunication("BELIIIN")
        })*/

        btnConnection.setOnClickListener(View.OnClickListener {
            try {
                pairDevices();
                enable_disable_Buttons()
            } catch (e: IOException) {
                e.printStackTrace();
            }
        })


    }

    /**
     * Trovare un modo per ricordare se i led sono accesi e da lì inviare i comandi di accensione in base al led scelto
     * o usando il nome o il numero della porta
     */
    private fun switchLed(led_num: Int, btn: Button){
        val message: String = "led{$led_num="
        var led: String = ""

        if(led_num == 1){
            led = if(isLed1on) "on}" else "off}"
            message.plus(led)
            isLed1on = !isLed1on
        } else if(led_num == 2){
            led = if(isLed2on) "on}" else "off}"
            isLed2on = !isLed2on
        }
        val msg = "$message$led"
        //INVIO MESSAGGIO BLUETOOTH
        arduinoCommunication(msg)
        //btn.setBackgroundColor("#FFFFF".toInt())
    }

    /**
     * invio della quantità di fade al led specificato
     * @param fade_amount 0 .. 255
     */
    private fun fadeLed(led_num: Int, fade_amount: Int): View.OnClickListener? {
        val message: String = "fade{$led_num="
        val fade = if(fade_amount < 0) 0 else if(fade_amount > 255) 255 else fade_amount
        message.plus("$fade}");
        if(led_num == 1) fade_amount1 = fade else if(led_num == 2) fade_amount2 = fade
        //INVIO DEL FADE

        arduinoCommunication(message)
        updateTextView()
        return null
    }



    /**
     * invio dell'attivazione dell'irrigazione
     *
     * (nel controller del bottone va disattivato per n minuti se si ha ancora il controllo)
     *
     * penso che faccio una roba del tipo da 0 .. 30 anche qui disattivando i bottoni quando vanno avanti e indietro
     */
    private fun irrigationStart(): View.OnClickListener? {
        val vel = if(irrigation_velocity < 0) 0 else if(irrigation_velocity > 30) 30 else irrigation_velocity
        val message: String = "irrigazione{$vel}"
        //INVIO DELL'IRRIGAZIONE
        arduinoCommunication(message)
        updateTextView()
        return null
    }

    private fun setIrrigationVel(){
    }

    /**
     * Funzione per l'allarme
     */
    private fun disableAlarm(){
    }

    /**
     * Funzione per controllare la disponibilità dei tasti se il sistema è in allarme, non si è ancora connesso o se è connesso
     */
    private fun enable_disable_Buttons(){
        for (el in 0..layout.childCount ){
            val child = layout.getChildAt(el)
            child?.isEnabled = true
        }
        btnConnection.isEnabled = false
        btnBell.isEnabled = false

    }


    private fun pairDevices() {
        ConnectToDevice(this).execute()
        btnLed1.isEnabled = true
        if (!btAdapter.isEnabled) {
            val turnOn = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            startActivityForResult(turnOn, BLUETOOTH_ON)
        }
    }

    private fun arduinoCommunication(input: String) {
        try{
            Log.i("BELIN", input.toByteArray().toString())
            socket!!.outputStream.write(input.toByteArray())
        } catch(e: IOException) {

            Log.i("coomutazzio", "NOH")
            e.printStackTrace()
        }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)

        /*led3.setText(Integer.toString(fade_amount1))
        led4.setText(Integer.toString(fade_amount2))
        irrigazione.setText(Integer.toString(irrigation_velocity))*/

        if (requestCode == BLUETOOTH_ON && resultCode == RESULT_OK) {
            load()
        }
    }

    private fun updateTextView() {
        led3.setText(this.fade_amount1)
        led4.setText(this.fade_amount2)
        irrigazione.setText(this.irrigation_velocity)
    }

    private fun scan() {
        if (!btAdapter.isEnabled) {
            val turnOn = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            startActivityForResult(turnOn, BLUETOOTH_ON)
        }
    }

    private fun load() {
        dispositivi = btAdapter.bondedDevices
        adapter.clear()
        for (bt in dispositivi){
            val s: String = "${ bt.name } - ${ bt.address }"
            print(s)
            adapter.add(s)
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
                if (socket == null || !socket?.isConnected!!) {
                    btAdapter = BluetoothAdapter.getDefaultAdapter()
                    val device: BluetoothDevice = btAdapter.getRemoteDevice(hc05_address)
                    socket = device.createInsecureRfcommSocketToServiceRecord(mUUID)
                    Log.i("CONNESSO", socket.toString())
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery()
                    socket?.connect()
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
                Log.i("data", "BELIN"+ socket.toString())
            }
            m_progress.dismiss()
        }
    }

    private fun createDialog(testoh: String) {
        Log.d("MainActivity", testoh)
    }


}