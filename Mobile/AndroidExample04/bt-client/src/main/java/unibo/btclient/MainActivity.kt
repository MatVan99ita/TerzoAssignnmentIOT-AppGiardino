package unibo.btclient

import android.app.ProgressDialog
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.content.Context
import android.content.Intent
import android.os.AsyncTask
import android.os.Bundle
import android.os.Handler
import android.os.StrictMode
import android.os.StrictMode.ThreadPolicy
import android.support.constraint.ConstraintLayout
import android.support.v7.app.AppCompatActivity
import android.util.Log
import android.widget.*
import com.google.gson.Gson
import okhttp3.OkHttpClient
import okhttp3.Request
import okhttp3.RequestBody
import okhttp3.RequestBody.Companion.toRequestBody
import unibo.btclient.databinding.ActivityMainBinding
import java.io.IOException
import java.io.InputStream
import java.util.*
import kotlin.time.Duration
import kotlin.time.DurationUnit
import kotlin.time.ExperimentalTime


class MainActivity : AppCompatActivity() {
    companion object {
        private const val SERVER_SOCKET = "http://192.168.11.158:8000/"
        private const val BLUETOOTH_ON = 1000
        private const val hc05_address: String = "98:D3:61:F6:6F:D3"
        private val mUUID: UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
        private var btAdapter: BluetoothAdapter = BluetoothAdapter.getDefaultAdapter()
        private var dispositivi: Set<BluetoothDevice> = HashSet()
        private val hc05: BluetoothDevice = btAdapter.getRemoteDevice(hc05_address);
        private lateinit var adapter: ArrayAdapter<Any?>
        private var arduino_status = "AUTO"
        private var socket: BluetoothSocket? = null
        private var pairing_code = 1234

    }

    inner class ArduinoStatus(val status: String = "NULL")

    private lateinit var binding: ActivityMainBinding
    private var response: String = ""

    private var fade_amount1: Int = 0
    private var fade_amount2: Int = 0
    private var isLed1on: Boolean = false
    private var isLed2on: Boolean = false
    private var irrigation_velocity: Int = 0

    private val MINUTE = 60000
    private val WAIT_TIME = 2

    private lateinit var layout: ConstraintLayout

    @OptIn(ExperimentalTime::class)
    private val TEMPO = Duration.convert(5.0, DurationUnit.MINUTES, DurationUnit.MILLISECONDS).toLong()




    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val policy = ThreadPolicy.Builder().permitAll().build()
        StrictMode.setThreadPolicy(policy)

        binding = ActivityMainBinding.inflate(layoutInflater)

        setContentView(binding.root)

        binding.fadeval1.text = "$fade_amount1"
        binding.fadeval2.text = "$fade_amount2"
        //binding.irrigationView.text = "$irrigation_velocity"

        //btAdapter = BluetoothAdapter.getDefaultAdapter()
        //lv = findViewById<View>(R.id.listview) as ListView
        adapter = ArrayAdapter<Any?>(this, android.R.layout.simple_list_item_1)
        //lv!!.adapter = adapter


        for (el in 0..binding.layoutBrutto.childCount ){
            val child = binding.layoutBrutto.getChildAt(el)
            child?.isEnabled = false
        }
        this.scan()
        binding.btConnectionReq.isEnabled = true

        this.addClickEvent()
    }


    /**
    FORMAT:
    [{LEDB, LEDF}]_[PIN]_[FADE_VALUE]
    IRRI_[SPEED]
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
                val statuuus = Gson()
                    .fromJson (
                        getArduinoStatus("arduino/status/").toString(),
                        ArduinoStatus::class.java
                    )

                Log.e("PORCADDIO", "$statuuus")

                Log.e("PORCADDIO", statuuus.status)

                pairDevices();
                if(statuuus.status != "ERROR") {
                    enable_disable_Buttons()
                    sendStaticRequest("arduino/status/MANUAL")
                }
                else {
                    binding.alarmBtn.isEnabled = true
                }


            } catch (e: IOException) {
                e.printStackTrace();
            }
        }

        binding.btConnectionSreq.setOnClickListener {
            ConnectToDevice.disconnect()
            for (el in 0..binding.layoutBrutto.childCount ){
                val child = binding.layoutBrutto.getChildAt(el)
                child?.isEnabled = false
            }
            binding.btConnectionReq.isEnabled = true
            sendStaticRequest("arduino/status/AUTO")
        }

        binding.ledf1Slider.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener{
            override fun onProgressChanged(p0: SeekBar?, progress: Int, p2: Boolean) {
                fade_amount1 = progress
                binding.fadeval1.text = "Send Fade_1: $progress"
            }
            override fun onStartTrackingTouch(p0: SeekBar?) {}
            override fun onStopTrackingTouch(p0: SeekBar?) {}
        })

        binding.ledf2Slider.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener{
            override fun onProgressChanged(p0: SeekBar?, progress: Int, p2: Boolean) {
                fade_amount2 = progress
                binding.fadeval2.text = "Send Fade_2: $progress"
            }
            override fun onStartTrackingTouch(p0: SeekBar?) {}
            override fun onStopTrackingTouch(p0: SeekBar?) {}
        })

        binding.irrigationView.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(p0: SeekBar?, progress: Int, p2: Boolean) {
                irrigation_velocity = progress
                binding.irriSpeedTxt.text = "$progress"
            }
            override fun onStartTrackingTouch(p0: SeekBar?) {}
            override fun onStopTrackingTouch(p0: SeekBar?) {}

        })


        binding.startIrriBtn.setOnClickListener {
            irrigationStart()
        }

        binding.alarmBtn.setOnClickListener {
            disableAlarm()
        }

        binding.fadeval1.setOnClickListener {
            arduinoCommunication("LEDF_1_$fade_amount1")

            binding.fadeval1.isEnabled = false
            binding.ledf1Slider.isEnabled = false

            Handler().postDelayed(
                {
                    binding.fadeval1.isEnabled = true
                    binding.ledf1Slider.isEnabled = true
                }, 2000
            )
        }

        binding.fadeval2.setOnClickListener {
            arduinoCommunication("LEDF_2_$fade_amount2")

            binding.fadeval2.isEnabled = false
            binding.ledf2Slider.isEnabled = false

            Handler().postDelayed(
                {
                    binding.fadeval2.isEnabled = true
                    binding.ledf2Slider.isEnabled = true
                }, 2000
            )
        }

    }


    private fun switchLed(led_num: Int){


        if(led_num == 1) {

            if(!isLed1on) {
                arduinoCommunication("LEDB_1_1")
                isLed1on = true
            } else if(isLed1on){
                arduinoCommunication("LEDB_1_0")
                isLed1on = false
            }

            binding.switch1.isEnabled = false
            Handler().postDelayed(
                { binding.switch1.isEnabled = true }, 2000
            )

        }

        if(led_num == 2) {
            if(!isLed2on) {
                arduinoCommunication("LEDB_2_1")
                isLed2on = true
            } else if(isLed2on){
                arduinoCommunication("LEDB_2_0")
                isLed2on = false
            }

            binding.switch2.isEnabled = false
            Handler().postDelayed(
                { binding.switch2.isEnabled = true }, 2000
            )
        }
    }

    /**
     * invio dell'attivazione dell'irrigazione
     *
     * (nel controller del bottone va disattivato per n minuti se si ha ancora il controllo)
     *
     * penso che faccio una roba del tipo da 0 .. 30 anche qui disattivando i bottoni quando vanno avanti e indietro
     */
    private fun irrigationStart(){

        val message: String = "IRRI_$irrigation_velocity"
        //INVIO DELL'IRRIGAZIONE
        arduinoCommunication("IRRI_$irrigation_velocity")
        lock_irrigation()
    }

    private fun lock_irrigation() {

        binding.irrigationView.isEnabled = false
        binding.startIrriBtn.isEnabled = false

        Handler().postDelayed(
            {
                binding.irrigationView.isEnabled = true
                binding.startIrriBtn.isEnabled = true
            },
            5000 //WAIT_TIME * MINUTE
        )
    }

    /**
     * Funzione per l'allarme
     */
    private fun disableAlarm(){
        sendStaticRequest("arduino/status/MANUAL")
        enable_disable_Buttons()
    }

    /**
     * Funzione per controllare la disponibilità dei tasti se il sistema è in allarme, non si è ancora connesso o se è connesso
     */
    private fun enable_disable_Buttons(){
        for (el in 0..binding.layoutBrutto.childCount ){
            val child = binding.layoutBrutto.getChildAt(el)
            child?.isEnabled = true
        }
        //binding.btConnectionReq.isEnabled = false
        binding.alarmBtn.isEnabled = false
        binding.btConnectionReq.isEnabled = true

    }




    @Throws(IOException::class)
    fun getArduinoStatus(endpoint: String): String? {

        val client: OkHttpClient = OkHttpClient()

        val url = "$SERVER_SOCKET$endpoint"

        val request = Request.Builder()
            .url(url).get().build()

        client.newCall(request).execute().use { response ->
            return response.body?.string()
        }

    }



    fun sendStaticRequest(endpoint: String): String? {
        // Crea il client OkHttp
        val client = OkHttpClient()

        // Costruisci la richiesta
        val request = Request.Builder()
            .url("$SERVER_SOCKET$endpoint")
            .post("".toRequestBody(null)) // Corpo vuoto per POST
            .build()

        client.newCall(request).execute().use { response ->
            return response.body?.string()
        }
    }


    private fun pairDevices() {

        //val myIntent: Intent = Intent(this@MainActivity, Banana::class.java)
        //this@MainActivity.startActivity(myIntent)

        ConnectToDevice(this).execute()

        binding.switch1.isEnabled = true


        if (!btAdapter.isEnabled) {
            if(arduino_status != "ERROR") {
                Log.e("MADIOBELVA", arduino_status)
                sendStaticRequest("arduino/status/MANUAL")
            }
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
            // Chiudi il socket e prova a riconnettere
            try {
                socket?.close()
            } catch (closeException: IOException) {
                Log.i("coomutazzio", "Errore nella chiusura del socket")
                closeException.printStackTrace()
            }
            socket = null  // Imposta il socket a null in modo da poterlo riconnettere in seguito

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
                    device.setPin("1234".toByteArray())
                    device.createBond()
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


        companion object {
            fun disconnect() {
                socket?.close()
            }
        }
    }
}