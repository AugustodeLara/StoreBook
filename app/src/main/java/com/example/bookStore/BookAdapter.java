package com.example.bookStore;

import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.example.curlssl.R;
import com.squareup.picasso.Picasso;

import java.util.ArrayList;
import java.util.List;

public class BookAdapter extends RecyclerView.Adapter<BookAdapter.ViewHolder> {

    private Context context;
    private List<String> books = new ArrayList<>();

    // No construtor, atribua o contexto
    public BookAdapter(List<String> books, Context context) {
        this.books = books;
        this.context = context;
    }

    // Adicione este método à classe BookAdapter
    public void addBook(String id, String title, String description, String thumbnailUrl) {
        // Adicione os detalhes do livro à lista
        books.add(id);
        books.add(title);
        books.add(description);
        books.add(thumbnailUrl);

        // Notifique o adaptador sobre a adição de um novo item
        notifyItemInserted(books.size() / 4 );
    }

    public static class ViewHolder extends RecyclerView.ViewHolder {
        public TextView textTitle;
        public TextView textDescription;
        public ImageView imageThumbnail;

        public ViewHolder(@NonNull View itemView) {
            super(itemView);
            textTitle = itemView.findViewById(R.id.textTitle);
            textDescription = itemView.findViewById(R.id.textDescription);
            imageThumbnail = itemView.findViewById(R.id.imageThumbnail);
        }
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.item_book, parent, false);
        return new ViewHolder(view);
    }

    private String removeDoubleAsterisks(String input) {
        return input.replace("**", "");
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        Log.d("BookAdapter", "onBindViewHolder - tamanho: " + books.size());
        Log.d("BookAdapter", "XXXXXXXXXXXXXXXXXXXXXXXX - position -: " + position);

        // Certifique-se de que há dados suficientes para preencher um conjunto completo
        if (position * 6 < books.size()) {
            String item1 = books.get(position * 6);  // ID
            String item2 = books.get(position * 6 + 1);  // Título
            String item3 = books.get(position * 6 + 2);  // Descrição
            String item4 = books.get(position * 6 + 3);  // URL da Imagem
            String item5 = books.get(position * 6 + 4);  // Favorito
            String item6 = books.get(position * 6 + 5);  // BuyLink

            holder.textTitle.setText(removeDoubleAsterisks(item2));
            holder.textDescription.setText(removeDoubleAsterisks(item3));
            Picasso.get().load(removeDoubleAsterisks(item4)).into(holder.imageThumbnail);

            // Configurar o clique no item do RecyclerView
            holder.itemView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    // Abrir a Activity de detalhes do livro
                    Intent intent = new Intent(context, BookDetailActivity.class);
                    intent.putExtra("id", removeDoubleAsterisks(item1));
                    intent.putExtra("title", removeDoubleAsterisks(item2));
                    intent.putExtra("description", removeDoubleAsterisks(item3));
                    intent.putExtra("thumbnailUrl", removeDoubleAsterisks(item4));
                    intent.putExtra("favorite", removeDoubleAsterisks(item5));  // Converte para booleano
                    intent.putExtra("buyLink", removeDoubleAsterisks(item6));
                    context.startActivity(intent);
                }
            });
        }
    }

    @Override
    public int getItemCount() {
        // Agora retorna o tamanho correto, considerando 6 elementos por posição
        return books.size() / 6;
    }
}
