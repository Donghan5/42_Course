// apps/backend/src/database/helpers.ts
import { getDatabase } from './db';

export { getDatabase };

export const dbGet = async (query: string, params: any[] = []): Promise<any> => {
    const db = await getDatabase();
    return new Promise((resolve, reject) => {
        db.get(query, params, (err, row) => {
            if (err) return reject(err);
            resolve(row);
        });
    });
};

export const dbAll = async (query: string, params: any[] = []): Promise<any[]> => {
    const db = await getDatabase();
    return new Promise((resolve, reject) => {
        db.all(query, params, (err, rows) => {
            if (err) return reject(err);
            resolve(rows);
        });
    });
};

export const dbRun = async (query: string, params: any[] = []): Promise<{ lastID: number; changes: number }> => {
    const db = await getDatabase();
    return new Promise((resolve, reject) => {
        db.run(query, params, function (err) {
            if (err) return reject(err);
            resolve({ lastID: this.lastID, changes: this.changes });
        });
    });
};

export const closeDatabase = async (): Promise<void> => {
    const db = await getDatabase();
    return new Promise((resolve, reject) => {
        db.close((err) => {
            if (err) return reject(err);
            resolve();
        });
    });
}
