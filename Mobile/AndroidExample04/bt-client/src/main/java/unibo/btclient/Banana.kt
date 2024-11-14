package unibo.btclient

import android.Manifest
import android.R
import android.app.Activity
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.content.pm.PackageManager
import android.os.Bundle
import android.support.v4.app.ActivityCompat
import android.support.v4.content.ContextCompat
import android.support.v7.app.AppCompatActivity
import android.view.View
import android.widget.AdapterView.OnItemClickListener
import android.widget.ArrayAdapter
import android.widget.ListView
import android.widget.Toast
import unibo.btclient.MainActivity
import unibo.btclient.databinding.BananaBinding

class Banana : AppCompatActivity() {

    private var bluetoothAdapter: BluetoothAdapter? = null
    private var deviceList: MutableList<String> = mutableListOf()
    private var arrayAdapter: ArrayAdapter<String>? = null

    private lateinit var bind: BananaBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        bind = BananaBinding.inflate(layoutInflater)
        setContentView(bind.root)

        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter()
        deviceList = ArrayList()
        arrayAdapter = ArrayAdapter(this, R.layout.simple_list_item_1, deviceList)

        bind.listview.adapter = arrayAdapter

        // Richiede il permesso della posizione (necessario per scansionare Bluetooth)
        if (ContextCompat.checkSelfPermission(
                this,
                Manifest.permission.ACCESS_FINE_LOCATION
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(
                this,
                arrayOf(Manifest.permission.ACCESS_FINE_LOCATION),
                REQUEST_PERMISSION_LOCATION
            )
        }

        // Setta il click listener sulla ListView per connettersi ai dispositivi
        bind.listview.onItemClickListener = OnItemClickListener { _, _, i, _ ->
            val item = deviceList[i]
            val address = item.substring(item.length - 17)
            val device = bluetoothAdapter!!.getRemoteDevice(address)
            connectToDevice(device)
        }
    }

    // Metodo per il pulsante "Cerca dispositivi"
    fun scan(view: View?) {
        if (bluetoothAdapter == null) {
            Toast.makeText(this, "Bluetooth non supportato", Toast.LENGTH_SHORT).show()
            return
        }

        if (!bluetoothAdapter!!.isEnabled) {
            val enableBtIntent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT)
        } else {
            startDiscovery()
        }
    }

    // Avvia la scoperta dei dispositivi
    private fun startDiscovery() {
        deviceList.clear()
        arrayAdapter!!.notifyDataSetChanged()

        // Registra il receiver per i dispositivi trovati
        val filter = IntentFilter(BluetoothDevice.ACTION_FOUND)
        registerReceiver(receiver, filter)

        bluetoothAdapter!!.startDiscovery()
        Toast.makeText(this, "Scansione avviata", Toast.LENGTH_SHORT).show()
    }

    // BroadcastReceiver per i dispositivi trovati
    private val receiver: BroadcastReceiver = object : BroadcastReceiver() {
        override fun onReceive(context: Context, intent: Intent) {
            val action = intent.action
            if (BluetoothDevice.ACTION_FOUND == action) {
                val device =
                    intent.getParcelableExtra<BluetoothDevice>(BluetoothDevice.EXTRA_DEVICE)
                val deviceInfo = """
                    ${device.name}
                    ${device.address}
                    """.trimIndent()
                if (!deviceList.contains(deviceInfo)) {
                    deviceList.

                    add(deviceInfo)
                    arrayAdapter!!.notifyDataSetChanged()
                }
            }
        }
    }

    // Metodo per connettersi al dispositivo selezionato
    private fun connectToDevice(device: BluetoothDevice) {
        try {
            // Metodo per pairing con password di default
            device.setPin("1234".toByteArray())
            device.createBond()
            Toast.makeText(this, "Connessione al dispositivo in corso", Toast.LENGTH_SHORT).show()


            // Torna a MainActivity dopo il pairing
            val intent = Intent(this@Banana, MainActivity::class.java)
            startActivity(intent)
            finish()
        } catch (e: Exception) {
            Toast.makeText(this, "Errore di connessione", Toast.LENGTH_SHORT).show()
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        unregisterReceiver(receiver)
    }

    // Gestione del risultato della richiesta permessi
    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<String>,
        grantResults: IntArray
    ) {
        if (requestCode == REQUEST_PERMISSION_LOCATION) {
            if (grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                Toast.makeText(this, "Permesso posizione garantito", Toast.LENGTH_SHORT).show()
            } else {
                Toast.makeText(
                    this,
                    "Permesso posizione necessario per la scansione",
                    Toast.LENGTH_SHORT
                ).show()
            }
        }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == REQUEST_ENABLE_BT) {
            if (resultCode == RESULT_OK) {
                startDiscovery()
            } else {
                Toast.makeText(this, "Bluetooth non abilitato", Toast.LENGTH_SHORT).show()
            }
        }
    }

    companion object {
        private const val REQUEST_ENABLE_BT = 1
        private const val REQUEST_PERMISSION_LOCATION = 2
    }
}