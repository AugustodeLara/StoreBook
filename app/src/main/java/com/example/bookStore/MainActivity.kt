package com.example.bookStore

import android.Manifest
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.widget.Button
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.example.curlssl.R
import java.io.FileOutputStream

class MainActivity : AppCompatActivity() {

    private val TAG = "MainActivity"
    private val WRITE_EXTERNAL_STORAGE_PERMISSION_CODE = 1

    private lateinit var cacert: String
    private lateinit var recyclerView: RecyclerView
    private lateinit var adapter: BookAdapter

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Solicitar permissões em tempo de execução
        requestWriteExternalStoragePermission()

        // Se as permissões já foram concedidas, continue com a inicialização
        if (hasWriteExternalStoragePermission()) {
            initialize()
        }
    }

    private fun initialize() {
        cacert = cacheCacertFile()

        recyclerView = findViewById(R.id.recyclerView)
        recyclerView.layoutManager = LinearLayoutManager(this)

        // Adicione logs para verificar o que está sendo retornado por getApiGoogleBook
        val apiGoogleBookResult = getApiGoogleBook(cacert)
        adapter = BookAdapter(apiGoogleBookResult.toMutableList(), this)
        recyclerView.adapter = adapter

        // Adicionar um OnClickListener ao botão "Favoritos"
        val favoritesButton = findViewById<Button>(R.id.favoritesButton)
        favoritesButton.setOnClickListener {
            if (hasSavedBook()) {
                openFavoritesActivity()
            } else {
                Toast.makeText(
                    this@MainActivity,
                    "Não há livros favoritados",
                    Toast.LENGTH_SHORT
                ).show()
            }
        }


    }

    private fun cacheCacertFile(): String {
        val path = cacheDir.resolve("cacert.pem").absolutePath

        try {
            assets.open("cacert.pem").use { inputStream ->
                FileOutputStream(path).use { outputStream ->
                    inputStream.copyTo(outputStream)
                }
            }
        } catch (e: Exception) {
            e.printStackTrace()
        }

        return path
    }

    // Método para verificar se a permissão de escrita no armazenamento externo foi concedida
    private fun hasWriteExternalStoragePermission(): Boolean {
        return ActivityCompat.checkSelfPermission(
            this,
            Manifest.permission.WRITE_EXTERNAL_STORAGE
        ) == PackageManager.PERMISSION_GRANTED
    }

    // Método para solicitar permissão de escrita no armazenamento externo
    private fun requestWriteExternalStoragePermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (!hasWriteExternalStoragePermission()) {
                ActivityCompat.requestPermissions(
                    this,
                    arrayOf(Manifest.permission.WRITE_EXTERNAL_STORAGE),
                    WRITE_EXTERNAL_STORAGE_PERMISSION_CODE
                )
            } else {
                // Permissões já concedidas, continue com a inicialização
                initialize()
            }
        }
    }

    private fun openFavoritesActivity() {
        val intent = Intent(this, FavoritesActivity::class.java)
        startActivity(intent)
    }

    private external fun getApiGoogleBook(cacert: String): Array<String>

    private external fun hasSavedBook(): Boolean

    companion object {
        init {
            System.loadLibrary("app")
        }
    }
}
