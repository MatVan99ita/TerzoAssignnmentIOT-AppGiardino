package esiot.module_lab_4_1;
import com.ngrok.*;
import com.ngrok.definitions.*;

import java.util.concurrent.CompletableFuture;
import java.util.concurrent.CompletionStage;

public class Example {
    public static void main(final String[] args) {
        final var ngrok = Ngrok.createDefault("2GP82TZ0U36oXijxzSo6rMYVO3M_6hDFYRTeDWL9MRQYeiTDL");

        System.out.println("Tunnels:");
        ngrok.tunnels().list().call()
                .thenCompose(tunnelPage -> printRecursively(ngrok, tunnelPage))
                .toCompletableFuture().join();

    }

    private static CompletionStage<Void> printRecursively(final Ngrok ngrok, final Page<TunnelList> currentPage) {
        currentPage.getPage().getTunnels().forEach(System.out::println);
        return currentPage.next().thenCompose(maybeTunnelPage -> maybeTunnelPage
                .map(tunnelPage -> printRecursively(ngrok, tunnelPage))
                .orElseGet(() -> CompletableFuture.<Void>completedFuture(null))
        );
    }
}
