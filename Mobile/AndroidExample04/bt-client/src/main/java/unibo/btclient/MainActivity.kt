package unibo.btclient

import android.annotation.SuppressLint
import android.app.ProgressDialog
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.content.ContentValues
import android.content.Context
import android.content.Intent
import android.os.AsyncTask
import android.os.Bundle
import android.os.Handler
import android.support.constraint.ConstraintLayout
import android.support.v7.app.AppCompatActivity
import android.util.Log
import android.view.View
import android.widget.*
import java.io.IOException
import java.io.InputStream
import java.net.URL
import java.util.*
import kotlin.time.Duration
import kotlin.time.DurationUnit
import kotlin.time.ExperimentalTime


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

    private var response: String = ""

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
    lateinit var txt : TextView

    private lateinit var layout: ConstraintLayout

    @OptIn(ExperimentalTime::class)
    private val TEMPO = Duration.convert(5.0, DurationUnit.MINUTES, DurationUnit.MILLISECONDS).toLong()

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
            * btnIrrigazione.setOnClickListener(new View.OnClickListener() {
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
            *
            *
            * btn.setOnClickListener(View.OnClickListener { v: View? ->
            btn.setEnabled(false)
            Handler().postDelayed({

                // This method will be executed once the timer is over
                btn.setEnabled(true)
                Log.d(ContentValues.TAG, "resend1")
            }, 2000) // set time as per your requirement
        })
            *
            * */

        led3 = findViewById(R.id.led3_view)
        led4 = findViewById(R.id.led4_view)
        irrigazione = findViewById(R.id.irrigation_view)

        led3.text = "$fade_amount1"
        led4.text = "$fade_amount2"
        irrigazione.text = "$irrigation_velocity"

        txt = findViewById(R.id.textResponse)
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
    FORMAT:
    [DEVICE]_[PIN]_[VALUE]
    DEVICE:
    LEDB,
    LEDF,
    IRRI
    PIN:
    Only 1, 2 (or 3 for fading both) for ledf
    VALUE:
    Light intensity for fading -> <value>
    Intensity of irrigation or activation/deactivation -> ON/OFF/<value>
     */
    /**
     * Funzione per assegnare ai vari bottoni la propria funzione
     */
    private fun addClickEvent() {
        btnLed1.setOnClickListener {
            switchLed(1, btnLed1)
        }
        btnLed2.setOnClickListener {
            switchLed(2, btnLed2)
        }

        btnConnection.setOnClickListener {
            try {
                pairDevices();
                enable_disable_Buttons()
            } catch (e: IOException) {
                e.printStackTrace();
            }
        }

        btnLed3up.setOnClickListener {
            setLedIntensity(3, "+")
        }
        btnLed3down.setOnClickListener {
            setLedIntensity(3, "-")
        }

        btnLed4up.setOnClickListener {
            setLedIntensity(4, "+")
        }
        btnLed4down.setOnClickListener {
            setLedIntensity(4, "-")
        }

        btnIrrigazione_fast.setOnClickListener {
            setIrrigationVel("+")
        }
        btnIrrigazione_slow.setOnClickListener {
            setIrrigationVel("-")
        }

        btnIrrigazione.setOnClickListener {
            btnIrrigazione.isEnabled = false
            irrigationStart()
            Handler().postDelayed({
                // This method will be executed once the timer is over
                btnIrrigazione.isEnabled = true
                Log.d(ContentValues.TAG, "resend1")
            }, TEMPO) // set time as per your requirement
        }

        btnBell.setOnClickListener {
            disableAlarm()
        }

    }

    private fun setIrrigationVel(s: String) {
        when(s){
            "+" -> if(irrigation_velocity < 30) irrigation_velocity++
            "-" -> if(irrigation_velocity > 0)  irrigation_velocity--
        }
        updateTextView()
    }

    /**
     * Trovare un modo per ricordare se i led sono accesi e da lì inviare i comandi di accensione in base al led scelto
     * o usando il nome o il numero della porta
     */
    private fun switchLed(led_num: Int, btn: Button){
        //INVIO MESSAGGIO BLUETOOTH
        arduinoCommunication("LEDB_$led_num")
    }

    /**
     * invio della quantità di fade al led specificato
     * @param fade_amount 0 .. 255
     */
    private fun fadeLed(led_num: Int){
        var message: String = "LEDF_"
        message += if (led_num == 3) "1_$fade_amount1" else if (led_num == 4) "2_$fade_amount2" else "3_${(fade_amount1+fade_amount2)/2}"
        //INVIO DEL FADE
        arduinoCommunication(message)
    }



    /**
     * invio dell'attivazione dell'irrigazione
     *
     * (nel controller del bottone va disattivato per n minuti se si ha ancora il controllo)
     *
     * penso che faccio una roba del tipo da 0 .. 30 anche qui disattivando i bottoni quando vanno avanti e indietro
     */
    private fun irrigationStart(){
        val vel = if(irrigation_velocity < 0) 0 else if(irrigation_velocity > 30) 30 else irrigation_velocity
        val message: String = "IRRI_1_$vel"
        //INVIO DELL'IRRIGAZIONE
        arduinoCommunication(message)
    }

    private fun setLedIntensity(led: Int, operation: String){
        when(operation){
            "+" -> if(led == 3 && fade_amount1 < 30) fade_amount1++ else if(led == 4 && fade_amount2 < 30) fade_amount2++
            "-" -> if(led == 3 && fade_amount1 > 0)  fade_amount1-- else if(led == 4 && fade_amount2 > 0)  fade_amount2--
        }
        updateTextView()
        fadeLed(led)
    }

    /**
     * Funzione per l'allarme
     */
    private fun disableAlarm(){
        arduinoCommunication("disable_alarm{1}")
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

    private fun getArduinoStatus(){
        var buffer = ByteArray(256)
        val inputStream: InputStream = socket!!.inputStream
        var bytes: Int
        try{
            bytes = inputStream.read(buffer)
            val readMessage: String = String(buffer, 1, bytes);
            // Send the obtained bytes to the UI Activity via handler
            Log.i("logging", readMessage + "");
            this.response = readMessage
        } catch (e: IOException) {
            Log.e("ERROR", e.toString())
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
        led3.setText(this.fade_amount1.toString())
        led4.setText(this.fade_amount2.toString())
        irrigazione.setText(this.irrigation_velocity.toString())
        this.txt.setText(response)
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
        //this.getArduinoStatus()
        Log.i("STATUS", response)
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
}