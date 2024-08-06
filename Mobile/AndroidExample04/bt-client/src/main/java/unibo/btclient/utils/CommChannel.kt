package unibo.btclient.utils

interface CommChannel {
    fun close()

    fun registerListener(listener: Listener?)

    fun removeListener(listener: Listener?)

    val remoteDeviceName: String?

    fun sendMessage(message: String?)

    interface Listener {
        fun onMessageReceived(receivedMessage: String?)
        fun onMessageSent(sentMessage: String?)
    }
}
