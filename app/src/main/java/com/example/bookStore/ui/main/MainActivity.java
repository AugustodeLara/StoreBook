package com.example.bookStore.ui.main;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.widget.Button;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.recyclerview.widget.GridLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.example.bookStore.ui.main.adapter.BookAdapter;
import com.example.bookStore.ui.favorites.FavoritesActivity;
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

        // Request permissions at runtime
        requestWriteExternalStoragePermission();

        // Continue with initialization if permissions are already granted
        if (hasWriteExternalStoragePermission()) {
            initialize();
        }
    }

    private void initialize() {
        // Cache the cacert file
        cacert = cacheCacertFile();

        // Set up RecyclerView with a GridLayoutManager
        recyclerView = findViewById(R.id.recyclerView);
        recyclerView.setLayoutManager(new GridLayoutManager(this, 2));

        // Fetch Google Book API results
        apiGoogleBookResult = Arrays.asList(getApiGoogleBook(cacert));

        // Use the results to initialize the adapter
        adapter = new BookAdapter(apiGoogleBookResult, this);
        recyclerView.setAdapter(adapter);

        // Set an OnClickListener for the "Favorites" button
        Button favoritesButton = findViewById(R.id.favoritesButton);
        favoritesButton.setOnClickListener(view -> {
            if (hasSavedBook()) {
                openFavoritesActivity();
            } else {
                Toast.makeText(
                        MainActivity.this,
                        "No favorited books",
                        Toast.LENGTH_SHORT
                ).show();
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

    // Check if the write external storage permission is granted
    private boolean hasWriteExternalStoragePermission() {
        return ContextCompat.checkSelfPermission(
                this, Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED;
    }

    // Request write external storage permission
    private void requestWriteExternalStoragePermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (!hasWriteExternalStoragePermission()) {
                ActivityCompat.requestPermissions(
                        this,
                        new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE},
                        WRITE_EXTERNAL_STORAGE_PERMISSION_CODE
                );
            } else {
                // Permissions already granted, continue with initialization
                initialize();
            }
        }
    }

    private void openFavoritesActivity() {
        Intent intent = new Intent(this, FavoritesActivity.class);
        startActivity(intent);
    }

    // JNI method to get Google Book API results
    private native String[] getApiGoogleBook(String cacert);

    // JNI method to check if there are saved books
    private native boolean hasSavedBook();

    static {
        System.loadLibrary("app");
    }

    // Handle the response of the permission request
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == WRITE_EXTERNAL_STORAGE_PERMISSION_CODE) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                // Permission granted, continue with initialization
                initialize();
            } else {
                // Permission denied, handle as needed (e.g., display an error message)
                Toast.makeText(this, "Permission is necessary for the app to function properly.", Toast.LENGTH_SHORT).show();
                finish();
            }
        }
    }
}
