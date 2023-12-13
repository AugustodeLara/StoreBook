package com.example.bookStore;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.recyclerview.widget.GridLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import com.example.curlssl.R;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";
    private static final int WRITE_EXTERNAL_STORAGE_PERMISSION_CODE = 1;

    private String cacert;
    private RecyclerView recyclerView;
    private BookAdapter adapter;

    private List<String> apiGoogleBookResult;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Solicitar permissões em tempo de execução
        requestWriteExternalStoragePermission();

        // Se as permissões já foram concedidas, continue com a inicialização
        if (hasWriteExternalStoragePermission()) {
            initialize();
        }
    }

    private void initialize() {
        cacert = cacheCacertFile();

        recyclerView = findViewById(R.id.recyclerView);
        recyclerView.setLayoutManager(new GridLayoutManager(this, 2)); // Definindo o GridLayoutManager com 2 colunas

        // Change the type of apiGoogleBookResult to List<String>
        apiGoogleBookResult = Arrays.asList(getApiGoogleBook(cacert));

        // Use apiGoogleBookResult as a List<String>
        adapter = new BookAdapter(apiGoogleBookResult, this);
        recyclerView.setAdapter(adapter);

        // Adicionar um OnClickListener ao botão "Favoritos"
        Button favoritesButton = findViewById(R.id.favoritesButton);
        favoritesButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (hasSavedBook()) {
                    openFavoritesActivity();
                } else {
                    Toast.makeText(
                            MainActivity.this,
                            "Não há livros favoritados",
                            Toast.LENGTH_SHORT
                    ).show();
                }
            }
        });
    }

    private String cacheCacertFile() {
        String path = getCacheDir().getAbsolutePath() + "/cacert.pem";

        try (InputStream inputStream = getAssets().open("cacert.pem");
             FileOutputStream outputStream = new FileOutputStream(path)) {
            byte[] buffer = new byte[1024];
            int read;
            while ((read = inputStream.read(buffer)) != -1) {
                outputStream.write(buffer, 0, read);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        return path;
    }

    // Método para verificar se a permissão de escrita no armazenamento externo foi concedida
    private boolean hasWriteExternalStoragePermission() {
        return ContextCompat.checkSelfPermission(
                this, Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED;
    }

    // Método para solicitar permissão de escrita no armazenamento externo
    private void requestWriteExternalStoragePermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (!hasWriteExternalStoragePermission()) {
                ActivityCompat.requestPermissions(
                        this,
                        new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE},
                        WRITE_EXTERNAL_STORAGE_PERMISSION_CODE
                );
            } else {
                // Permissões já concedidas, continue com a inicialização
                initialize();
            }
        }
    }

    private void openFavoritesActivity() {
        Intent intent = new Intent(this, FavoritesActivity.class);
        startActivity(intent);
    }

    private native String[] getApiGoogleBook(String cacert);

    private native boolean hasSavedBook();

    static {
        System.loadLibrary("app");
    }

    // Tratar a resposta da solicitação de permissão
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == WRITE_EXTERNAL_STORAGE_PERMISSION_CODE) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                // Permissão concedida, continue com a inicialização
                initialize();
            } else {
                // Permissão negada, trate conforme necessário (ex: exiba uma mensagem de erro)
                Toast.makeText(this, "A permissão é necessária para o aplicativo funcionar corretamente.", Toast.LENGTH_SHORT).show();
                finish();
            }
        }
    }
}
