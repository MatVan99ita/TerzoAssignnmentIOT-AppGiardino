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
import android.util.AttributeSet
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.*
import unibo.btclient.databinding.ActivityMainBinding
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

    private lateinit var binding: ActivityMainBinding
    private var response: String = ""

    private var fade_amount1: Int = 0
    private var fade_amount2: Int = 0
    private var isLed1on: Boolean = false
    private var isLed2on: Boolean = false
    private var irrigation_velocity: Int = 0

    private lateinit var layout: ConstraintLayout

    @OptIn(ExperimentalTime::class)
    private val TEMPO = Duration.convert(5.0, DurationUnit.MINUTES, DurationUnit.MILLISECONDS).toLong()




    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)

        setContentView(binding.root)

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

        binding.led3View.text = "$fade_amount1"
        binding.led4View.text = "$fade_amount2"
        binding.irrigationView.text = "$irrigation_velocity"

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
        binding.btConnectionReq.isEnabled = true

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
        binding.switch1.setOnClickListener {
            switchLed(1)
        }
        binding.switch2.setOnClickListener {
            switchLed(2)
        }

        binding.btConnectionReq.setOnClickListener {
            try {
                pairDevices();
                enable_disable_Buttons()
            } catch (e: IOException) {
                e.printStackTrace();
            }
        }

        binding.led3plus.setOnClickListener {
            setLedIntensity(3, "+")
        }
        binding.led3minus.setOnClickListener {
            setLedIntensity(3, "-")
        }

        binding.led4plus.setOnClickListener {
            setLedIntensity(4, "+")
        }
        binding.led4minus.setOnClickListener {
            setLedIntensity(4, "-")
        }

        binding.irrigationPlus.setOnClickListener {
            setIrrigationVel("+")
        }
        binding.irrigationMinus.setOnClickListener {
            setIrrigationVel("-")
        }

        binding.irrigationOnoff.setOnClickListener {
            binding.irrigationOnoff.isEnabled = false
            irrigationStart()
            Handler().postDelayed({
                // This method will be executed once the timer is over
                binding.irrigationOnoff.isEnabled = true
                Log.d(ContentValues.TAG, "resend1")
            }, TEMPO) // set time as per your requirement
        }

        binding.alarmBtn.setOnClickListener {
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
     * TODO: rivedere i messaggi manuali
     */
    private fun switchLed(led_num: Int){
        //INVIO MESSAGGIO BLUETOOTH
        arduinoCommunication("LEDBM_$led_num")
    }

    /**
     * invio della quantità di fade al led specificato
     * @param fade_amount 0 .. 255 <-> 0 .. 5
     */
    private fun fadeLed(led_num: Int){
        var message: String = "LEDFM_"
        message += if (led_num == 3) "1_$fade_amount1"
                   else if (led_num == 4) "2_$fade_amount2" else "BAnANA"
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
        val message: String = "IRRIM_$vel"
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
        binding.btConnectionReq.isEnabled = false
        binding.alarmBtn.isEnabled = false

    }


    private fun pairDevices() {
        ConnectToDevice(this).execute()
        binding.switch1.isEnabled = true
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
        binding.led3View.text = this.fade_amount1.toString()
        binding.led4View.text = this.fade_amount2.toString()
        binding.irrigationView.text = this.irrigation_velocity.toString()
        binding.textResponse.text = response
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



    private class ConnectToDevice(val context: Context) : AsyncTask<Void, Void, String>() {
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